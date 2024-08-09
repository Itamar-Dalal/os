#include "pch.h"
#include "memory.h"
#include "multiboot_info.h"

extern virtaddr_t end; // Defined by the linker
virtaddr_t placement_address = (virtaddr_t)&end;

void *kmalloc(size_t size) {
	// Align the size to the nearest multiple of 4
	if (size % 4 != 0) {
		size = (size + 3) & ~0x3;
	}
	// Save the current placement address
	virtaddr_t addr = placement_address;
	placement_address += size;
	// Return the old placement address
	return (void *)addr;
}

// Allocates memory that is aligned to a page boundary 
// and provides the physical address of the allocated memory
void *kmalloc_ap(size_t size, physaddr_t *phys_addr) {
	if (placement_address & (PAGE_SIZE - 1)) {
		placement_address &= ~(PAGE_SIZE - 1);
		placement_address += PAGE_SIZE;
	}
	virtaddr_t addr = placement_address;
	if (phys_addr)
		*phys_addr = addr; // 1:1 mapping (identity mapping)
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

virtaddr_t *page_directory __attribute__((aligned(PAGE_SIZE)));
virtaddr_t *page_tables[NUM_OF_PAGE_TABLES] __attribute__((aligned(PAGE_SIZE)));

// Function prototypes for assembly routines
extern void loadPageDirectory(physaddr_t *);
extern void enablePaging();

void init_vmm() {
	// Allocate space for the page directory
	page_directory = (virtaddr_t *)kmalloc_ap(NUM_OF_PAGE_TABLES * sizeof(virtaddr_t), NULL);
	memset_tool((uint8_t *)page_directory, 0, NUM_OF_PAGE_TABLES * sizeof(virtaddr_t));

	// Map memory for the kernel (from 0x00000000 to &end)
	virtaddr_t kernel_end_addr = (virtaddr_t)&end;
	uint32_t num_kernel_pages = (kernel_end_addr + PAGE_SIZE - 1) / PAGE_SIZE;

	for (size_t i = 0; i < (num_kernel_pages / NUM_OF_PAGES_IN_TABLE) + 1; i++) {
		page_tables[i] = (virtaddr_t *)kmalloc_ap(NUM_OF_PAGES_IN_TABLE * sizeof(virtaddr_t), NULL);
		for (size_t j = 0; j < NUM_OF_PAGES_IN_TABLE; j++) {
			physaddr_t page_addr = (i * NUM_OF_PAGES_IN_TABLE + j) * PAGE_SIZE;
			if (page_addr >= kernel_end_addr) {
				break;
			}
			page_tables[i][j] = page_addr | 3; // Present, read/write
		}
		page_directory[i] = ((physaddr_t)page_tables[i]) | 3; // Present, read/write
	}

	// Identity mapping for kernel space (assumes kernel virtual addresses start at 0xC0000000)
	for (size_t i = 768; i < 768 + (num_kernel_pages / NUM_OF_PAGES_IN_TABLE) + 1; i++) {
		page_tables[i] = (virtaddr_t *)kmalloc_ap(NUM_OF_PAGES_IN_TABLE * sizeof(virtaddr_t), NULL);
		for (size_t j = 0; j < NUM_OF_PAGES_IN_TABLE; j++) {
			physaddr_t page_addr = ((i - 768) * NUM_OF_PAGES_IN_TABLE + j) * PAGE_SIZE;
			if (page_addr >= kernel_end_addr) {
				break;
			}
			page_tables[i][j] = page_addr | 3; // Present, read/write
		}
		page_directory[i] = ((physaddr_t)page_tables[i]) | 3; // Present, read/write
	}

	// Load the page directory into CR3 and enable paging
	loadPageDirectory((physaddr_t *)page_directory);
	enablePaging();
}