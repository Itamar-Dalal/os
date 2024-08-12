#ifndef MEMORY_H
#define MEMORY_H

#define BLOCK_SIZE 0x1000 // 4KB
#define TOTAL_BLOCKS (total_memory / BLOCK_SIZE)
#define MEMORY_MAP_SIZE (TOTAL_BLOCKS / 8)

#define PAGE_SIZE 0x1000 // 4KB
#define MEMORY_MAP_SIZE (TOTAL_BLOCKS / 8)
#define KERNEL_START_VADDR 0xC0000000  // Kernel starts at 3GB
#define NUM_OF_PAGES_IN_TABLE 1024
#define NUM_OF_PAGE_TABLES 1024

#define PAGE_DIRECTORY_INDEX(x) (((x) >> 22) & 0x3FF)
#define PAGE_TABLE_INDEX(x) (((x) >> 12) & 0x3FF)

// PMM
void pmm_init();
void *pmm_alloc_block();
void pmm_free_block(void *ptr);

// VMM
typedef struct {
    uint32_t m_entries[NUM_OF_PAGES_IN_TABLE];
} page_table;

typedef struct {
    uint32_t m_entries[NUM_OF_PAGE_TABLES];
} page_directory;

void vmm_init();
void vmm_map_page(void *phys, void *virt);
void *kmalloc(size_t size);
void *kmalloc_ap(size_t size, physaddr_t *phys_addr);

#endif // MEMORY_H
