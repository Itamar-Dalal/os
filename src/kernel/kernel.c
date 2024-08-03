#include "pch.h"
#include "descriptors.h"
#include "multiboot_info.h"
#include "memory.h"


int kmain(void *mbd, uint32_t magic) {
    screen_clear();
    if (magic != 0x2BADB002) {
        screen_print("Invalid multiboot header.\n", 0);
        return -1;
    }
    multiboot_info_t *mb_info = (multiboot_info_t *)mbd;
    uint32_t mem_lower = mb_info->mem_lower; // In kilobytes
    uint32_t mem_upper = mb_info->mem_upper; // In kilobytes
    total_memory = (mem_lower + mem_upper) * 1024; // Convert to bytes
    pmm_init();
    screen_print("------------------------- OS developed by Itamar Dalal -------------------------\n", 0xF0);
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

    while (true) {;}
    return 0;
}
