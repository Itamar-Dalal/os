#include "screen.h"
#include "descriptors.h"

int kmain(void *mbd, unsigned int magic) {
	screen_clear();
	if (magic != 0x2BADB002) {
		screen_print("Invalid multiboot header.\n");
		return -1;
	}
	screen_print("hello world!\n");
	screen_print("Setting up the GDT.\n");
	gdt_setup();
	screen_print("GDT set.\n");
	return 0;
}