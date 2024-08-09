#ifndef MEMORY_H
#define MEMORY_H

#define BLOCK_SIZE 0x1000 // 4KB
#define TOTAL_BLOCKS (total_memory / BLOCK_SIZE)
#define MEMORY_MAP_SIZE (TOTAL_BLOCKS / 8)

#define PAGE_SIZE 0x1000 // 4KB
#define NUM_OF_PAGES_IN_TABLE 1024
#define NUM_OF_PAGE_TABLES 1024

void pmm_init();
void *pmm_alloc_block();
void pmm_free_block(void *ptr);

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
	page_t pages[NUM_OF_PAGES_IN_TABLE];
} page_table_t;

typedef struct {
	page_table_t *tables[NUM_OF_PAGE_TABLES]; // Array of pointers to page tables
	physaddr_t tablesPhysical[NUM_OF_PAGE_TABLES]; // Array that holds the physical address of the corresponding page table
	physaddr_t physicalAddr; // The physical address of the page directory
} page_directory_t;

void vmm_init();
void switch_page_directory(page_directory_t *dir);
void enable_paging();
page_t *get_page(virtaddr_t addr, bool new_page_table, page_directory_t *dir);
void alloc_frame(page_t *page, bool is_kernel, bool is_writeable);

#endif