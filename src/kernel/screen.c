#include "pch.h"

#define DEFAULT_ATTR 0x07

uint8_t *vidmem = (uint8_t *)0xb8000;
uint8_t x = 0;
uint8_t y = 0;
uint32_t prompt_length = 0;
uint8_t prompt_x = 0;
uint8_t prompt_y = 0;

void screen_clear() {
    int i;
    for (i = 0; i < NUM_OF_BYTES(NUM_OF_CELLS_X, NUM_OF_CELLS_Y); i += 2) {
        *(vidmem + i) = 0x20;
        *(vidmem + i + 1) = DEFAULT_ATTR;
    }
    x = 0;
    y = 0;
}

void set_cursor(uint8_t x, uint8_t y) {
    uint16_t pos = y * NUM_OF_CELLS_X + x;
    // Send the high byte of the cursor position to VGA port
    outb(0x3D4, 0x0E);
    outb(0x3D5, (pos >> 8) & 0xFF);
    // Send the low byte of the cursor position to VGA port
    outb(0x3D4, 0x0F);
    outb(0x3D5, pos & 0xFF);
}

void set_prompt(char *prompt, uint8_t attr) {
    if (attr == 0)
        attr = DEFAULT_ATTR;
    prompt_length = 0;
    while (prompt[prompt_length]) {
        *(vidmem + (y * NUM_OF_CELLS_X * 2) + (x * 2)) = prompt[prompt_length];
        *(vidmem + (y * NUM_OF_CELLS_X * 2) + (x * 2) + 1) = attr;
        x++;
        prompt_length++;
    }
    prompt_y = y;
    prompt_x = x - 1;
}

void screen_scroll() {
    size_t i;
    // Shift all characters up by one line in video memory by copying data from the current line to the line above
    for (i = NUM_OF_SCROLL_BYTES; i < NUM_OF_BYTES(NUM_OF_CELLS_X, NUM_OF_CELLS_Y); i += 2) {
        *(vidmem + i - NUM_OF_SCROLL_BYTES) = *(vidmem + i);
        *(vidmem + i - NUM_OF_SCROLL_BYTES + 1) = *(vidmem + i + 1);
    }
    // Clear the last line
    for (i = 0; i < NUM_OF_CELLS_X * 2; i += 2) {
        *(vidmem + (NUM_OF_CELLS_Y - 1) * NUM_OF_CELLS_X * 2 + i) = ' ';
        *(vidmem + (NUM_OF_CELLS_Y - 1) * NUM_OF_CELLS_X * 2 + i + 1) = DEFAULT_ATTR;
    }
    y--;
    prompt_y--;
}

void handle_input() {
    // This function will be called when Enter is pressed
    // Currently, it does nothing
}

void screen_print(char *string, uint8_t attr) {
    if (attr == 0)
        attr = DEFAULT_ATTR; // Defult attribute
    size_t i = 0;
    while (string[i]) {
        if (y == NUM_OF_CELLS_Y)
            screen_scroll();
        if (string[i] > 0x1f && string[i] != 0x7f) { // Printable characters
            *(vidmem + (y * NUM_OF_CELLS_X * 2) + (x * 2)) = string[i];
            *(vidmem + (y * NUM_OF_CELLS_X * 2) + (x * 2) + 1) = attr;
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
                        *(vidmem + (y * NUM_OF_CELLS_X * 2) + (x * 2) + 1) = attr;
                    }
                    else if (y > 2) {
                        x = NUM_OF_CELLS_X - 1;
                        y--;
                        *(vidmem + (y * NUM_OF_CELLS_X * 2) + (x * 2)) = ' ';
                        *(vidmem + (y * NUM_OF_CELLS_X * 2) + (x * 2) + 1) = attr;
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
                if (y == NUM_OF_CELLS_Y)
                    screen_scroll();
                set_prompt("> ", 0);
                break;
            }
        }
        i++;
        set_cursor(x, y);
    }
}

void screen_print_int(int32_t num, uint32_t base, uint8_t attr) {
    if (attr == 0)
        attr = DEFAULT_ATTR;
    char buff[100];
    itoa(num, buff, base);
    screen_print(buff, attr);
}
