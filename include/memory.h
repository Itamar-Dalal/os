#ifndef MEMORY_H
#define MEMORY_H

#define BLOCK_SIZE 0x1000 // 4KB
#define TOTAL_BLOCKS (total_memory / BLOCK_SIZE)
#define MEMORY_MAP_SIZE (TOTAL_BLOCKS / 8)

void pmm_init();
void *pmm_alloc_block();
void pmm_free_block(void *ptr);

#define PAGE_SIZE 0x1000 // 4KB
#define NUM_OF_PAGES_IN_TABLE 1024
#define NUM_OF_PAGE_TABLES 1024

void init_vmm();

#endif