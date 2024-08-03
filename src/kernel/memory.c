#include "pch.h"
#include "memory.h"
#include "multiboot_info.h"

#define BLOCK_SIZE 4096 // 4KB
#define TOTAL_BLOCKS (total_memory / BLOCK_SIZE)
#define MEMORY_MAP_SIZE (TOTAL_BLOCKS / 8)

extern uint32_t end; // Defined by the linker
uint32_t placement_address = (uint32_t)&end;

void *kmalloc(size_t size) {
	// Align the size to the nearest multiple of 4
	if (size % 4 != 0) {
		size &= ~0x3;
		size += 4;
	}

	// Save the current placement address
	uint32_t addr = placement_address;
	placement_address += size;

	// Return the old placement address
	return (void *)addr;
}

uint8_t *memory_map; // Bitmap data structure, each bit represents a block of memory

void pmm_init() {
	memory_map = (uint8_t *)kmalloc(MEMORY_MAP_SIZE);
	memset_tool(memory_map, 0, MEMORY_MAP_SIZE); // Mark all memory blocks as free
	pmm_alloc_block(); // Allocation of the first block for initialization
}

void *pmm_alloc_block() {
	for (size_t i = 0; i < MEMORY_MAP_SIZE; i++) {
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
	size_t bit_index = block_index % 8;
	memory_map[byte_index] &= ~(1 << bit_index); // Free the block bit (0)
}