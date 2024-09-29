#ifndef FAT_H
#define FAT_H

// FAT16 implementation


// FAT16 BIOS Parameter Block (BPB)
typedef struct
{
    uint8_t jump_instruction[3]; // Jump to bootstrap code
    uint8_t oem_id[8];           // OEM (Original Equipment Manufacturer) identifier (e.g., "MSDOS5.0" for FAT16)
    uint16_t bytes_per_sector;   // 512 bytes
    uint8_t sectors_per_cluster; // Number of sectors per cluster
    uint16_t reserved_sectors;   // Number of reserved sectors (usually 1 for the boot sector)
    uint8_t fat_count;           // Number of FAT tables (usually 2 in FAT16)
    uint16_t root_entry_count;   // Number of root directory entries (usually 224)
    uint16_t total_sectors_16;   // Total number of sectors (FAT16). This is limited to 65536 sectors (approximately 32MB)
    uint8_t media_type;          // Media descriptor
    uint16_t fat_size_16;        // Sectors per FAT (FAT16)
    uint16_t sectors_per_track;  // Sectors per track for BIOS (usually used for older media types like floppy disks)
    uint16_t number_of_heads;    // Number of heads for BIOS
    uint32_t hidden_sectors;     // Number of hidden sectors before the partition
} __attribute__((packed)) BPB;

void initialize_bpb(BPB *bpb, uint32_t total_sectors, uint16_t sectors_per_fat, uint8_t sectors_per_cluster);

#endif