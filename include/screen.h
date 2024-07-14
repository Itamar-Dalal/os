#ifndef SCREEN_H
#define SCREEN_H

#define NUM_OF_CELLS_X 80
#define NUM_OF_CELLS_Y 25
#define NUM_OF_BYTES(x_cells, y_cells) x_cells * y_cells * 2 // 80 * 25 cells * 2 bytes for each cell
#define NUM_OF_SCROLL_BYTES 160

void screen_clear();
void screen_print(char *);
void set_prompt(char *);
void screen_print_int(int32_t num, uint32_t base);

#endif