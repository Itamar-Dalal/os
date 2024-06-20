#include "screen.h"

int kmain(void *mbd, unsigned int magic) {
	screen_clear();
	if (magic != 0x2BADB002) {
		screen_print("Invalid multiboot header.");
		return -1;
	}
	screen_print("hello world!");
	return 0;
}