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

    // Extended BIOS Parameter Block (EBPB)
    uint8_t drive_number;           // BIOS drive number
    uint8_t reserved1;              // Reserved for padding (usually 0)
    uint8_t boot_signature;         // Extended boot signature (0x29)
    uint32_t volume_serial_number;  // Volume serial number
    uint8_t volume_label[11];       // Volume label
    uint8_t fs_type[8];             // File system type (FAT16)

    // Boot code (remaining bytes up to 510)
    uint8_t boot_code[448];         // Boot code placeholder

    uint16_t signature;             // Boot sector signature (0x55AA)
} __attribute__((packed)) BPB;

void initialize_bpb(BPB *bpb, uint16_t total_sectors, uint16_t sectors_per_fat, uint8_t sectors_per_cluster);
void create_boot_sector(BPB *bpb);
void initialize_fat_tables(BPB *bpb);
void initialize_root_directory(BPB *bpb);
uint16_t read_cluster(BPB *bpb, uint16_t cluster_index);
void write_cluster(BPB *bpb, uint16_t cluster_number, uint16_t value);
uint16_t find_free_cluster(BPB *bpb);
void create_file(BPB *bpb, const char *filename);
void read_file(BPB *bpb, const char *filename, uint8_t *buffer, uint32_t buffer_size);
void delete_file(BPB *bpb, const char *filename);

#endif