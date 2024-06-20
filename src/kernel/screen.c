#include "screen.h"

unsigned char *vidmem = (unsigned char *)0xb8000;
unsigned char x = 0;
unsigned char y = 0;

void screen_clear(){
	int i;
	for (i = 0; i < NUM_OF_BYTES(NUM_OF_CELLS_X, NUM_OF_CELLS_Y); i += 2) {
		*(vidmem + i) = 0x20;
		*(vidmem + i + 1) = 0x07;
	}
	x = 0;
	y = 0;
}

void screen_scroll() {
	int i;
	for (i = NUM_OF_SCROLL_BYTES; i < NUM_OF_BYTES(NUM_OF_CELLS_X, NUM_OF_CELLS_Y); i += 2) {
		*(vidmem + i - NUM_OF_SCROLL_BYTES) = *(vidmem + i);
	}
}

void screen_print(char* string) {
	int i = 0;
	while (string[i]) {
		if (y == NUM_OF_CELLS_Y) {
			y = 24;
			screen_scroll();
		}
		if (string[i] > 0x1f && string[i] != 0x7f) { // Printable characters
			*(vidmem + (y * NUM_OF_CELLS_X * 2) + (x * 2)) = string[i];
			x++;
			if (x == NUM_OF_CELLS_X) {
				x = 0;
				y++;
			}
		}
		else {
			switch (string[i]) {
				case 0x08: // Backspace
					if (x != 0)
						x--;
					break;
				
				case 0x09: // Tab
					x += 4;
					if (x >= 80) {
						x = 0;
						y++;
					}
					break;

				case 0x0a: // Line feed
					x = 0;
					y++;
					break;
			}
		}
		i++;
	}
}