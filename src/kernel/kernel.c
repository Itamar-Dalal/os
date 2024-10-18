#include "pch.h"
#include "descriptors.h"
#include "multiboot_info.h"
#include "memory.h"

#define MULTIBOOT_MAGIC 0x2BADB002

void test_pmm() {
    void *block1 = pmm_alloc_block();
    if (block1 != NULL) {
        screen_print("Allocated block1 at address: ", 0);
        screen_print_int((uint32_t)block1, 16, 0);
        screen_print("\n", 0);
    }

    void *block2 = pmm_alloc_block();
    if (block2 != NULL) {
        screen_print("Allocated block2 at address: ", 0);
        screen_print_int((uint32_t)block2, 16, 0);
        screen_print("\n", 0);
    }

    pmm_free_block(block1);
    screen_print("Freed block1 at address: ", 0);
    screen_print_int((uint32_t)block1, 16, 0);
    screen_print("\n", 0);

    void *block3 = pmm_alloc_block();
    if (block3 != NULL) {
        screen_print("Allocated block3 at address: ", 0);
        screen_print_int((uint32_t)block3, 16, 0);
        screen_print("\n", 0);
    }

    void *block4 = pmm_alloc_block();
    if (block4 != NULL) {
        screen_print("Allocated block4 at address: ", 0);
        screen_print_int((uint32_t)block4, 16, 0);
        screen_print("\n", 0);
    }
}
void test_vmm() {
    virtaddr_t test_virt_addr = 0xE0000000;
    physaddr_t test_phys_addr = 0x100000;
    // Test mapping a physical page to a virtual address
    screen_print("Mapping physical address to virtual address.\n", 0);
    vmm_map_page((void *)test_phys_addr, (void *)test_virt_addr);

    // Write a test value to the virtual address
    uint32_t *test_ptr = (uint32_t *)test_virt_addr;
    *test_ptr = 0x12345678;

    // Read the value back to verify
    if (*test_ptr == 0x12345678) {
        screen_print("VMM Test: Virtual to physical mapping is working.\n", 0);
    }
    else {
        screen_print("VMM Test: Virtual to physical mapping failed.\n", 0);
    }
}

int kmain(void *mbd, uint32_t magic) {
    screen_clear();
    if (magic != MULTIBOOT_MAGIC) {
        screen_print("Invalid multiboot header.\n", 0);
        return EXIT_FAILURE;
    }
    multiboot_info_t *mb_info = (multiboot_info_t *)mbd;
    uint32_t mem_lower = mb_info->mem_lower; // In kilobytes
    uint32_t mem_upper = mb_info->mem_upper; // In kilobytes
    total_memory = (mem_lower + mem_upper) * 1024; // Convert to bytes
    screen_print("------------------------- OS developed by Itamar Dalal -------------------------\n", 0xF0);
    pmm_init();
    test_pmm();
    vmm_init();
    test_vmm();
    //screen_print("Setting up the GDT.\n", 0);
    gdt_setup();
    //screen_print("GDT set.\n", 0);
    //screen_print("Setting up the IDT.\n", 0);
    idt_setup();
    //screen_print("IDT set.\n", 0);
    //screen_print("Sending interrupt.\n", 0); // For checking the IDT is working
    //__asm__("int $0x00");
    //__asm__("int $0x80");
    //__asm__("int $0x23");
    
    while (true) {;}
    return EXIT_SUCCESS;
}
