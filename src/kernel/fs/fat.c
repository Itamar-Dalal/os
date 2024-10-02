#include "pch.h"
#include "fat.h"

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
}

int create_boot_sector(const char *filename, BPB *bpb) {

}

