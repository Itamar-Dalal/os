#include "pch.h"
#include "descriptors.h"

// Multiboot information structure (taken from the gnu.org website: https://www.gnu.org/software/grub/manual/multiboot/multiboot.html)
typedef struct {
    uint32_t flags;
    uint32_t mem_lower;
    uint32_t mem_upper;
    uint32_t boot_device;
    uint32_t cmdline;
    uint32_t mods_count;
    uint32_t mods_addr;

    uint32_t mmap_length;
    uint32_t mmap_addr;

    uint32_t drives_length;
    uint32_t drives_addr;

    uint32_t config_table;
    uint32_t boot_loader_name;
    uint32_t apm_table;

    uint32_t vbe_control_info;
    uint32_t vbe_mode_info;
    uint32_t vbe_mode;
    uint32_t vbe_interface_seg;
    uint32_t vbe_interface_off;
    uint32_t vbe_interface_len;

    uint32_t framebuffer_addr;
    uint32_t framebuffer_pitch;
    uint32_t framebuffer_width;
    uint32_t framebuffer_height;
    uint32_t framebuffer_bpp;
    uint32_t framebuffer_type;

    uint32_t framebuffer_palette_addr;
    uint16_t framebuffer_palette_num_colors;

} multiboot_info_t;

int kmain(void *mbd, uint32_t magic) {
    // multiboot_info_t *mb_info = (multiboot_info_t *)mbd;
    screen_clear();
    if (magic != 0x2BADB002) {
        screen_print("Invalid multiboot header.\n", 0);
        return -1;
    }
    screen_print("------------------------- OS developed by Itamar Dalal -------------------------\n", 0x1E);
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
    return 0;
}
