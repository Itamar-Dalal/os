#include "pch.h"
#include "fat.h"
#include "ata.h"
#include "memory.h"

// FAT16 implementation

#define RESERVED_SECTORS 1
#define NUM_OF_FAT 2
#define NUM_OF_ROOT_ENTRIES 224
#define MEDIA_TYPE 0xF8 // Fixed disk
#define SECTORS_PER_TRACK 63
#define NUM_OF_HEADS 255
#define NUM_OF_HIDDEN_SECTORS 0
#define DRIVER_NUM 0x80 // First hard drive
#define EBPB_SIGNATURE 0x29
#define VOLUME_LABEL "MYDISK     "
#define FILE_SYSTEM_TYPE "FAT16   "
#define BOOT_SECTOR_SIGNATURE 0xAA55

#define ROOT_DIR_ENTRY_SIZE 32 // Each entry in the root directory is 32 bytes

#define FAT16_EOF 0xFFF8
#define FAT16_FREE 0x0000

// FAT16 structure: https://www.hdd-tool.com/pic/fat16.jpg

void initialize_bpb(BPB *bpb, uint16_t total_sectors, uint16_t sectors_per_fat, uint8_t sectors_per_cluster) {
    bpb->jump_instruction[0] = 0xEB; // Jump instruction
    bpb->jump_instruction[1] = 0x3C; // Offset (60 bytes - the BPB is 62 bytes including the jump instruction that is 2 bytes)
    bpb->jump_instruction[2] = 0x90; // NOP
    memcpy_tool(bpb->oem_id, "MSWIN4.1", 8);

    // Basic disk paramters
    bpb->bytes_per_sector = BYTES_PER_SECTOR;
    bpb->sectors_per_cluster = sectors_per_cluster;
    bpb->reserved_sectors = RESERVED_SECTORS;
    bpb->fat_count = NUM_OF_FAT;
    bpb->root_entry_count = NUM_OF_ROOT_ENTRIES;
    bpb->total_sectors_16 = total_sectors;
    bpb->media_type = MEDIA_TYPE;
    bpb->fat_size_16 = sectors_per_fat;
    bpb->sectors_per_track = SECTORS_PER_TRACK;
    bpb->number_of_heads = NUM_OF_HEADS;
    bpb->hidden_sectors = NUM_OF_HIDDEN_SECTORS;

    // Extended BIOS Parameter Block (EBPB)
    bpb->drive_number = DRIVER_NUM;
    bpb->reserved1 = 0x00;
    bpb->boot_signature = EBPB_SIGNATURE;
    bpb->volume_serial_number = generate_volume_serial();
    memcpy_tool(bpb->volume_label, VOLUME_LABEL, sizeof(VOLUME_LABEL) - 1);
    memcpy_tool(bpb->fs_type, FILE_SYSTEM_TYPE, sizeof(FILE_SYSTEM_TYPE) - 1);
    // Boot code (initilize to 0 because GRUB doesn't rely on the FAT16 boot sector's boot code)
    memset_tool(bpb->boot_code, 0x00, sizeof(bpb->boot_code));
    bpb->signature = BOOT_SECTOR_SIGNATURE;
}

// This Function will return a new random number for each call (volume serial number should be unique)
uint32_t generate_volume_serial() {
    // Linear Congruential Generator (LCG) algorithm
    static uint32_t seed = 12345;
    seed = (seed * 1103515245) + 12345;
    return seed;
}

int32_t create_boot_sector(BPB *bpb) {
    uint8_t boot_sector[BYTES_PER_SECTOR];
    memset_tool(boot_sector, 0, sizeof(boot_sector));
    memcpy_tool(boot_sector, bpb, sizeof(BPB));
    // Write the boot sector to the first block (LBA 0) of the disk
    int32_t return_code = ata_write_block(0x0, boot_sector);
    if (return_code != 0) {
        screen_print("Error in create_boot_sector: failed to write boot sector to disk", 0);
        return -1;
    }
    return 0;
}

int32_t initialize_fat_tables(BPB *bpb) {
    uint32_t fat_size = bpb->fat_size_16 * bpb->bytes_per_sector;
    uint8_t *fat = (uint8_t *)kmalloc(fat_size);
    if (fat == NULL) {
        screen_print("Error in initialize_fat_tables: failed to allocate memory for FAT", 0);
        return -1;
    }
    memset_tool(fat, 0, fat_size);
    // In FAT16, the first two entries are reserved
    fat[0] = bpb->media_type;
    fat[1] = 0xFF;
    fat[2] = 0xFF;
    fat[3] = 0xFF;

    uint32_t fat_lba;
    for (size_t i = 0; i < bpb->fat_count; i++) {
        fat_lba = bpb->reserved_sectors + (bpb->fat_size_16 * i);
        for (size_t j = 0; j < bpb->fat_size_16; j++) {
            int32_t return_code = ata_write_block(fat_lba + j, (fat + (bpb->bytes_per_sector * j)));
            if (return_code != 0) {
                screen_print("Error in initialize_fat_tables: failed to write FAT to disk", 0);
                //kfree(fat);
                return -1;
            }
        }
    }
    //kfree(fat);
    return 0;
}

int32_t initialize_root_directory(BPB *bpb) {
    uint32_t root_dir_lba = bpb->reserved_sectors + (bpb->fat_size_16 * bpb->fat_count);

    // Root Directory structure: http://osr600doc.sco.com/en/FS_admin/_The_Root_Directory.html
    uint32_t root_dir_size = bpb->root_entry_count * ROOT_DIR_ENTRY_SIZE;
    uint8_t *root_directory = (uint8_t *)kmalloc(root_dir_size);
    if (root_directory == NULL) {
        screen_print("Error in initialize_root_directory: failed to allocate memory for root directory", 0);
        return -1;
    }
    memset_tool(root_directory, 0, root_dir_size);
    int32_t return_code = ata_write_block(root_dir_lba, root_directory);
    if (return_code != 0) {
        screen_print("Error in initialize_root_directory: failed to write root directory to disk", 0);
        //kfree(root_directory);
        return -1;
    }
    //kfree(root_directory);
    return 0;
}

uint16_t read_cluster(BPB *bpb, const uint16_t cluster_number) {
    uint32_t fat_start_address = bpb->reserved_sectors * bpb->bytes_per_sector;
    uint32_t fat_offset = cluster_number * 2; // Each FAT16 entry is 2 bytes
    uint32_t fat_sector = fat_start_address + (fat_offset / bpb->bytes_per_sector); // Determine which sector to read

    uint16_t fat_sector_data[bpb->bytes_per_sector / 2];
    int32_t return_code = ata_read_block(fat_sector, fat_sector_data);
    if (return_code != 0) {
        screen_print("Error in read_cluster: failed to read FAT sector for the cluster", 0);
        return 0xFFFF; // Invalid value
    }

    uint32_t entry_offset = (fat_offset % bpb->bytes_per_sector) / 2;
    uint16_t next_cluster = fat_sector_data[entry_offset];
    return next_cluster;
}

int32_t write_cluster(BPB *bpb, const uint16_t cluster_number, const uint16_t value) {
    uint32_t fat_start_address = bpb->reserved_sectors * bpb->bytes_per_sector;
    uint32_t fat_offset = cluster_number * 2; // Each FAT16 entry is 2 bytes
    uint32_t fat_sector = fat_start_address + (fat_offset / bpb->bytes_per_sector); // Determine which sector to read
    
    // Read the FAT sector, modify the target cluster entry, and rewrite the FAT sector to disk
    uint16_t fat_sector_data[bpb->bytes_per_sector / 2];
    int32_t return_code = ata_read_block(fat_sector, fat_sector_data);
    if (return_code != 0) {
        screen_print("Error in write_cluster: failed to read FAT sector for the cluster", 0);
        return -1;
    }

    fat_sector_data[(fat_offset % bpb->bytes_per_sector) / 2] = value;
    return_code = ata_write_block(fat_sector, fat_sector_data);
    if (return_code != 0) {
        screen_print("Error in write_cluster: failed to write FAT entry for the cluster", 0);
        return -1;
    }
    return 0;
}

uint16_t find_free_cluster(BPB *bpb) {
    for (uint16_t cluster_index = 2; cluster_index < 0xFFF8; cluster_index++) {
        if (read_cluster(bpb, cluster_index) == FAT16_FREE) { // Free cluster is marked as 0
            return cluster_index;
        }
    }
    return 0; // No free cluster found
}

void create_file(BPB *bpb, const char *filename) {

}

void read_file(BPB *bpb, const char *filename, uint8_t *buffer, uint32_t buffer_size) {

}

void delete_file(BPB *bpb, const char *filename) {

}