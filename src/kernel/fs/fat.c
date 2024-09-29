#include "pch.h"
#include "fat.h"

void initialize_bpb(BPB *bpb, uint32_t total_sectors, uint16_t sectors_per_fat, uint8_t sectors_per_cluster) {
    bpb->jump_instruction[0] = 0xEB; // Jump instruction
    bpb->jump_instruction[1] = 0x3C; // Offset (60 bytes - the BPB is 62 bytes including the jump instruction that is 2 bytes)
    bpb->jump_instruction[2] = 0x90; // NOP
}


