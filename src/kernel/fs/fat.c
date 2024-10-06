#include "pch.h"
#include "fat.h"
#include "ata.h"
#include "memory.h"

// FAT16 structure: https://www.hdd-tool.com/pic/fat16.jpg

void initialize_bpb(BPB *bpb, uint16_t total_sectors, uint16_t sectors_per_fat, uint8_t sectors_per_cluster) {
    bpb->jump_instruction[0] = 0xEB; // Jump instruction
    bpb->jump_instruction[1] = 0x3C; // Offset (60 bytes - the BPB is 62 bytes including the jump instruction that is 2 bytes)
    bpb->jump_instruction[2] = 0x90; // NOP
    memcpy_tool(bpb->oem_id, "MSWIN4.1", 8);

    // Basic disk paramters
    bpb->bytes_per_sector = 512;
    bpb->sectors_per_cluster = sectors_per_cluster;
    bpb->reserved_sectors = 1;
    bpb->fat_count = 2;
    bpb->root_entry_count = 224;
    bpb->total_sectors_16 = total_sectors;
    bpb->media_type = 0xF8; // Fixed disk
    bpb->fat_size_16 = sectors_per_fat;
    bpb->sectors_per_track = 63;
    bpb->number_of_heads = 255;
    bpb->hidden_sectors = 0;

    // Extended BIOS Parameter Block (EBPB)
    bpb->drive_number = 0x80; // First hard drive
    bpb->reserved1 = 0x00;
    bpb->boot_signature = 0x29;
    bpb->volume_serial_number = generate_volume_serial();
    memcpy_tool(bpb->volume_label, "MYDISK     ", 11);
    memcpy_tool(bpb->fs_type, "FAT16   ", 8);
    // Boot code (initilize to 0 because GRUB doesn't rely on the FAT16 boot sector's boot code)
    memset_tool(bpb->boot_code, 0x00, sizeof(bpb->boot_code));
    bpb->signature = 0xAA55;
}

// This Function will return a new random number for each call (volume serial number should be unique)
uint32_t generate_volume_serial() {
    // Linear Congruential Generator (LCG) algorithm
    static uint32_t seed = 12345;
    seed = (seed * 1103515245) + 12345;
    return seed;
}

void create_boot_sector(BPB *bpb) {
    uint8_t boot_sector[512];
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
    for (int i = 0; i < bpb->fat_count; i++) {
        fat_lba = bpb->reserved_sectors + (bpb->fat_size_16 * i);
        ata_write_block(fat_lba, fat);
    }
}

void initialize_root_directory(BPB *bpb) {

}

uint16_t read_cluster(BPB *bpb, uint16_t cluster_index) {

}

void write_cluster(BPB *bpb, uint16_t cluster_number, uint16_t value) {

}

uint16_t find_free_cluster(BPB *bpb) {
    for (uint16_t cluster_index = 2; cluster_index < 0xFFF8; cluster_index++) {
        if (read_cluster(bpb, cluster_index) == 0x0000) { // Free cluster is marked as 0
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