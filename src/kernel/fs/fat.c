#include "pch.h"
#include "fat.h"
#include "ata.h"
#include "memory.h"

#define BYTES_PER_SECTOR 512
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

#define ROOT_DIR_ENTRY_SIZE 32

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

void create_boot_sector(BPB *bpb) {
    uint8_t boot_sector[BYTES_PER_SECTOR];
    memset_tool(boot_sector, 0, sizeof(boot_sector));
    memcpy_tool(boot_sector, bpb, sizeof(BPB));
    ata_write_block(0x0, boot_sector); // Write the boot sector to the first block (LBA 0) of the disk
}

void initialize_fat_tables(BPB *bpb) {
    uint32_t fat_size = bpb->fat_size_16 * bpb->bytes_per_sector;
    uint8_t *fat = (uint8_t *)kmalloc(fat_size);
    if (fat == NULL) {
        screen_print("Error in initialize_fat_tables: failed to allocate memory for fat.", 0);
        return;
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
        for (size_t j = 0; j < bpb->fat_size_16; j++)
            ata_write_block(fat_lba + j, fat + (bpb->bytes_per_sector * j));
    }
    //kfree(fat);
}

void initialize_root_directory(BPB *bpb) {
    uint32_t root_dir_lba = bpb->reserved_sectors + (bpb->fat_size_16 * bpb->fat_count);
    uint32_t root_dir_start_address = root_dir_lba * bpb->bytes_per_sector;

    // Root Directory structure: http://osr600doc.sco.com/en/FS_admin/_The_Root_Directory.html
    uint8_t *root_directory = (uint8_t *)kmalloc(bpb->root_entry_count * ROOT_DIR_ENTRY_SIZE); // Each entry is 32 bytes
    if (root_directory == NULL) {
        screen_print("Error in initialize_root_directory: failed to allocate memory for root directory.", 0);
        return;
    }
    memset_tool(root_directory, 0, bpb->root_entry_count * ROOT_DIR_ENTRY_SIZE);
}

uint16_t read_cluster(BPB *bpb, const uint16_t cluster_number) {
    uint32_t fat_start_address = bpb->reserved_sectors * bpb->bytes_per_sector;
    uint32_t fat_offset = cluster_number * 2; // Each FAT16 entry is 2 bytes
    uint32_t fat_sector = fat_start_address + (fat_offset / bpb->bytes_per_sector); // Determine which sector to read
    
    uint16_t fat_sector_data[BYTES_PER_SECTOR / 2];
    ata_read_block(fat_sector, fat_sector_data);
    uint32_t entry_offset = (fat_offset % bpb->bytes_per_sector) / 2;
    uint16_t next_cluster = fat_sector_data[entry_offset];

    return next_cluster;
}


void write_cluster(BPB *bpb, uint16_t cluster_number, uint16_t value) {

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