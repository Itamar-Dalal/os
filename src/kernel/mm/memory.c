#include "pch.h"
#include "memory.h"
#include "multiboot_info.h"

extern virtaddr_t end; // Defined by the linker
static virtaddr_t placement_address = (virtaddr_t)&end;

typedef struct memory_block {
    struct memory_block *next;
    bool is_free;
    size_t size;
} memory_block_t;

#define METADATA_SIZE sizeof(memory_block_t)

static memory_block_t *free_list = NULL;

void *kmalloc(size_t size) {
    if (!size)
        return NULL;
    // Align the size to the nearest multiple of 4
    if (size % 4 != 0)
        size = (size + 3) & ~0x3;

    // Add space for metadata (memory_block_t struct)
    size += METADATA_SIZE;
    // Search for the best fit
    memory_block_t *best_fit = NULL;
    memory_block_t *cur = free_list;
    while (cur) {
        if (cur->is_free && cur->size >= size) {
            if (best_fit == NULL || cur->size < best_fit->size) {
                best_fit = cur;
            }
        }
        cur = cur->next;
    }

    if (best_fit) {
        // If best fit block is larger than needed, split it
        if (best_fit->size > size) {
            memory_block_t *new_block = (memory_block_t *)((char *)best_fit + size);
            new_block->size = best_fit->size - size;
            new_block->is_free = true;
            new_block->next = best_fit->next;
            best_fit->next = new_block;
            best_fit->size = size;
        }
        best_fit->is_free = false;
        return (void *)((char *)best_fit + METADATA_SIZE);
    }

    // No suitable block found, allocate at the end of the placement address
    best_fit = (memory_block_t *)placement_address;
    best_fit->size = size;
    best_fit->is_free = false;
    best_fit->next = free_list;

    free_list = best_fit;
    placement_address += size;
    return (void *)((char *)best_fit + METADATA_SIZE);
}

void kfree(void *ptr) {
    if (ptr == NULL)
        return;

    memory_block_t *block = (memory_block_t *)((char *)ptr - METADATA_SIZE);
    block->is_free = true;

    // Mearge close free blocks
    memory_block_t *curr = block->next;
    while (curr != NULL && curr->is_free) {
        block->next = curr->next;
        block->size += curr->size;
        curr = block->next;
        
    }
}

// PMM implementation
static uint8_t *memory_map; // Bitmap data structure, each bit represents a block of memory

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

extern void loadPageDirectory(physaddr_t dir);
extern void enablePaging();

static page_directory *_cur_directory = NULL;
static physaddr_t _cur_pdbr = 0; // page directory base pointer

void vmm_init() {
    page_table *table = (page_table *)pmm_alloc_block();
    if (table == NULL)
        return;
    page_table *table2 = (page_table *)pmm_alloc_block();
    if (table2 == NULL)
        return;
    memset_tool(table, 0, sizeof(page_table));
    uint32_t page = (1 << 0); // Present bit
    // Low memory mapping (for example: video memory, BDA - BIOS data area and more)
    for (int i = 0, frame = 0x0; i < NUM_OF_PAGES_IN_TABLE; i++, frame += PAGE_SIZE)
        table2->m_entries[i] = page | (frame & ~0xFFF); // "& ~0xFFF" clears the lower 12 bits
    // Kernel memory mapping (GRUB loads the kernel at the physical address 0x100000 - 1MB)
    for (int i = 0, frame = 0x100000; i < NUM_OF_PAGES_IN_TABLE; i++, frame += PAGE_SIZE)
        table->m_entries[i] = page | (frame & ~0xFFF);

    page_directory *dir = (page_directory *)pmm_alloc_block();
    if (dir == NULL)
        return;
    memset_tool(dir, 0, sizeof(page_directory));

    uint32_t *entry = &dir->m_entries[PAGE_DIRECTORY_INDEX(KERNEL_START_VADDR)];
    *entry |= (1 << 0) | (1 << 1); // Present, Read/Write
    *entry |= ((uintptr_t)table & ~0xFFF);

    entry = &dir->m_entries[PAGE_DIRECTORY_INDEX(0x00000000)];
    *entry |= (1 << 0) | (1 << 1);
    *entry |= ((uintptr_t)table2 & ~0xFFF);

    _cur_pdbr = (physaddr_t)dir;
    _cur_directory = dir;

    loadPageDirectory(_cur_pdbr);
    enablePaging();
}

void vmm_map_page(void *phys, void *virt) {
    page_directory *pageDirectory = _cur_directory;
    uint32_t *entry = &pageDirectory->m_entries[PAGE_DIRECTORY_INDEX((virtaddr_t)virt)];
    page_table *table;
    if ((*entry & (1 << 0)) == 0) { // If the page is not present
        table = (page_table *)pmm_alloc_block();
        if (table == NULL)
            return;
        memset_tool(table, 0, sizeof(page_table));
        *entry = (uintptr_t)table | (1 << 0) | (1 << 1);
    }
    table = (page_table *)((*entry) & ~0xFFF);
    uint32_t *page = &table->m_entries[PAGE_TABLE_INDEX((virtaddr_t)virt)];
    *page = (physaddr_t)phys | (1 << 0); // Present
    // TLB invalidation (because I changed the mapping of the virtual address)
    __asm__ volatile("invlpg (%0)" ::"r" ((virtaddr_t)virt) : "memory");
    // Note that the MMU adds translations to the TLB automatically, so I don't need to change it
    // I just need to remove the invalidated translations (with the invlpg command)
}