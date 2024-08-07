#include "pch.h"
#include "memory.h"
#include "multiboot_info.h"

#define BLOCK_SIZE 0x1000 // 4KB
#define TOTAL_BLOCKS (total_memory / BLOCK_SIZE)
#define MEMORY_MAP_SIZE (TOTAL_BLOCKS / 8)

#define PAGE_SIZE 0x1000 // 4KB
#define NUM_OF_PAGES 1024
#define NUM_OF_PAGE_TABLES 1024

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

void *kmalloc_a(size_t size) {
	// Align the placement_address to PAGE_SIZE
	if (placement_address & (PAGE_SIZE - 1)) {
		placement_address &= ~(PAGE_SIZE - 1);
		placement_address += PAGE_SIZE;
	}
	uint32_t addr = placement_address;
	placement_address += size;
	return (void *)addr;
}

// PMM implementation
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

// VMM implementation
// For more info: https://wiki.osdev.org/Paging
typedef struct {
	uint32_t present : 1;
	uint32_t rw : 1;
	uint32_t user : 1;
	uint32_t accessed : 1;
	uint32_t dirty : 1;
	uint32_t unused : 7;
	uint32_t frame : 20; // 20 and not 32 bits because the other 12 bits are used as the offset
} page_t;

typedef struct {
	page_t pages[NUM_OF_PAGES];
} page_table_t;

typedef struct {
	page_table_t *tables[NUM_OF_PAGE_TABLES]; // Array of pointers to page tables
	physaddr_t tablesPhysical[NUM_OF_PAGE_TABLES]; // Array that holds the physical address of the corresponding page table
	physaddr_t physicalAddr; // The physical address of the page directory
} page_directory_t;

page_directory_t *kernel_directory;
page_directory_t *current_directory;

void vmm_init() {
	kernel_directory = (page_directory_t *)kmalloc_a(sizeof(page_directory_t));
	memset_tool(kernel_directory, 0, sizeof(page_directory_t));
	current_directory = kernel_directory;
}

void switch_page_directory(page_directory_t *dir) {
	current_directory = dir;
	// Change the CR3 register to hold the value of the new page directory
	asm volatile("mov %0, %%cr3":: "r"(dir->physicalAddr));
}

void enable_paging() {
	uint32_t cr0;
	asm volatile("mov %%cr0, %0":: "=r"(cr0));
	cr0 |= 0x80000000; // Enable paging
	asm volatile("mov %0, %%cr0":: "r"(cr0));
}

page_t *get_page(virtaddr_t addr, bool make_new_page, page_directory_t *dir) {
	// Divide by page size (4 KB) to get the page number
	addr /= PAGE_SIZE;
	/* Divide the page number by the number of pages in the page table
	   to get the index of the page table in the page directory */
	size_t table_index = addr / NUM_OF_PAGES;
	// in progress

}