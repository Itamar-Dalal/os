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
}
