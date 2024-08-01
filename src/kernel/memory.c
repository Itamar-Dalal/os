#include "pch.h"
#include "memory.h"
#include "multiboot_info.h"

#define BLOCK_SIZE 4096 // 4KB
#define TOTAL_BLOCKS (total_memory / BLOCK_SIZE)

void example1() {
	screen_print_int(TOTAL_BLOCKS, 10, 0);
}