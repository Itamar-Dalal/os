#include "screen.h"
#include "tools.h"

unsigned char *vidmem = (unsigned char *)0xb8000;
unsigned char x = 0;
unsigned char y = 0;
unsigned int prompt_length = 0;
unsigned char prompt_x = 0;
unsigned char prompt_y = 0;

void screen_clear() {
    int i;
    for (i = 0; i < NUM_OF_BYTES(NUM_OF_CELLS_X, NUM_OF_CELLS_Y); i += 2) {
        *(vidmem + i) = 0x20;
        *(vidmem + i + 1) = 0x07;
    }
    x = 0;
    y = 0;
}

void set_prompt(char *prompt) {
    prompt_length = 0;
    while (prompt[prompt_length]) {
        *(vidmem + (y * NUM_OF_CELLS_X * 2) + (x * 2)) = prompt[prompt_length];
        x++;
        prompt_length++;
    }
    prompt_y = y;
    prompt_x = x - 1;
}

void screen_scroll() {
    int i;
    for (i = NUM_OF_SCROLL_BYTES; i < NUM_OF_BYTES(NUM_OF_CELLS_X, NUM_OF_CELLS_Y); i += 2) {
        *(vidmem + i - NUM_OF_SCROLL_BYTES) = *(vidmem + i);
    }
    y--;
}

void handle_input() {
    // This function will be called when Enter is pressed
    // Currently, it does nothing
}

void screen_print(char *string) {
    int i = 0;
    while (string[i]) {
        if (y == NUM_OF_CELLS_Y) {
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
                    if (y != prompt_y || x > prompt_x + 1) {
                        if (x != 0) {
                            x--;
                            *(vidmem + (y * NUM_OF_CELLS_X * 2) + (x * 2)) = ' ';
                        }
                        else if (y > 2) {
                            x = NUM_OF_CELLS_X - 1;
                            y--;
                            *(vidmem + (y * NUM_OF_CELLS_X * 2) + (x * 2)) = ' ';
                        }
                    }
                    break;

                case 0x09: // Tab
                    x += 4;
                    if (x >= NUM_OF_CELLS_X) {
                        x = 0;
                        y++;
                    }
                    break;

                case 0x0a: // Line feed (Enter key)
                    handle_input();
                    x = 0;
                    y++;
                    set_prompt("> ");
                    break;
            }
        }
        i++;
    }
}

void screen_print_int(int num, int base) {
    char buff[100];
    itoa(num, buff, base);
    screen_print(buff);
}
