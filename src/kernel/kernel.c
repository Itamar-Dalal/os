#include "screen.h"
#include "descriptors.h"

// Multiboot information structure (taken from the gnu.org website: https://www.gnu.org/software/grub/manual/multiboot/multiboot.html)
typedef struct {
    unsigned int flags;
    unsigned int mem_lower;
    unsigned int mem_upper;
    unsigned int boot_device;
    unsigned int cmdline;
    unsigned int mods_count;
    unsigned int mods_addr;

    unsigned int mmap_length;
    unsigned int mmap_addr;

    unsigned int drives_length;
    unsigned int drives_addr;

    unsigned int config_table;
    unsigned int boot_loader_name;
    unsigned int apm_table;

    unsigned int vbe_control_info;
    unsigned int vbe_mode_info;
    unsigned int vbe_mode;
    unsigned int vbe_interface_seg;
    unsigned int vbe_interface_off;
    unsigned int vbe_interface_len;

    unsigned int framebuffer_addr;
    unsigned int framebuffer_pitch;
    unsigned int framebuffer_width;
    unsigned int framebuffer_height;
    unsigned int framebuffer_bpp;
    unsigned int framebuffer_type;

    unsigned int framebuffer_palette_addr;
    unsigned short framebuffer_palette_num_colors;

} multiboot_info_t;

int kmain(void *mbd, unsigned int magic) {
    // multiboot_info_t *mb_info = (multiboot_info_t *)mbd;
    screen_clear();
    if (magic != 0x2BADB002) {
        screen_print("Invalid multiboot header.\n");
        return -1;
    }
    screen_print("Hello, world!\n");
    screen_print("Setting up the GDT.\n");
    gdt_setup();
    screen_print("GDT set.\n");
    return 0;
}
