#include "pch.h"
#include "memory.h"
#include "multiboot_info.h"

#define BLOCK_SIZE 4096 // 4KB
#define TOTAL_BLOCKS (total_memory / BLOCK_SIZE)

uint8_t memory_map[TOTAL_BLOCKS / 8]; // Bitmap data structure, each bit represents a block of memory

void pmm_init() {
	memset(memory_map, 0, sizeof(memory_map)) // Mark all memory blocks as free
}

void *pmm_alloc_block() {
	for (size_t i = 0; i < sizeof(memory_map); i++) {
		if (memory_map[i] != 0xFF) {
			for (size_t j = 0; j < 8; j++) {
				if (!(memory_map[i] & (1 << j))) { // Find the block that is free (0)
					memory_map[i] |= (1 << j); // Mark the block as used (1)
					return (void *)(BLOCK_SIZE * i * 8 + BLOCK_SIZE * j);
				}
			}
		}
	}
	return NULL; // If there is no free block
}

void pmm_free_block(void *ptr) {
	uint32_t addr = (uint32_t)ptr;
	size_t block_index = addr / BLOCK_SIZE;
	size_t byte_index = block_index / 8;
	size_t bit_index = block_index % 2;
	memory_map[byte_index] &= ~(1 << bit_index); // Free the block bit (0)
}