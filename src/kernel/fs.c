#include "pch.h"

struct superblock {
    uint32_t magic; // To identify the type of the file system
    uint32_t total_inodes; //
    uint32_t total_blocks;
    uint32_t free_inodes;
    uint32_t free_blocks;
    uint32_t block_size;
    uint32_t inode_size;
};
