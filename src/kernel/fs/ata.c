#include "pch.h"
#include "ata.h"

// For more info: https://wiki.osdev.org/ATA_PIO_Mode#28_bit_PIO

// ATA ports
#define ATA_DATA          0x1F0  // Data port
#define ATA_SECTOR_COUNT  0x1F2  // Sector count port
#define ATA_LBA_LOW       0x1F3  // LBA low byte
#define ATA_LBA_MID       0x1F4  // LBA mid byte
#define ATA_LBA_HIGH      0x1F5  // LBA high byte
#define ATA_DRIVE_SELECT  0x1F6  // Drive select
#define ATA_COMMAND       0x1F7  // Command port
#define ATA_STATUS        0x1F7  // Status port
#define ATA_ERROR         0x1F1  // Error register

#define ATA_CMD_READ  0x20       // Read command
#define ATA_CMD_WRITE 0x30       // Write command

#define ATA_STATUS_ERR  0x01     // Error bit
#define ATA_STATUS_DRQ  0x08     // Data request bit
#define ATA_STATUS_BSY  0x80     // Busy bit

// Error messages for debug
#define ATA_ERR_ABORTED_CMD  0x01  // Aborted Command
#define ATA_ERR_MEDIA_ERR    0x10  // Media error

void print_ata_error() {
    uint8_t error = inb(ATA_ERROR);
    if (error & ATA_ERR_ABORTED_CMD) // The drive couldn't process the command 
        screen_print("ATA Error: Command Aborted\n", 0);
    else if (error & ATA_ERR_MEDIA_ERR) // Corrupted disk data
        screen_print("ATA Error: Media Error\n", 0);
    else
        screen_print("ATA Error: Unknown Error\n", 0);
}

int32_t ata_read_block(uint32_t lba, void *buffer) {
    // Select the master drive (0xE0 for master, 0xF0 for slave)
    outb(ATA_DRIVE_SELECT, 0xE0 | ((lba >> 24) & 0x0F));
    // Set the sector count to 1 (read 1 sector)
    outb(ATA_SECTOR_COUNT, 1);
    // Set the LBA (Logical Block Addressing)
    outb(ATA_LBA_LOW, lba & 0xFF);
    outb(ATA_LBA_MID, (lba >> 8) & 0xFF);
    outb(ATA_LBA_HIGH, (lba >> 16) & 0xFF);
    // Send the read command
    outb(ATA_COMMAND, ATA_CMD_READ);

    // Wait for the drive to be ready (check BSY and DRQ bits in status register)
    while (inb(ATA_STATUS) & ATA_STATUS_BSY);  // Wait while BSY (busy) bit is set
    if (inb(ATA_STATUS) & ATA_STATUS_ERR) {
        print_ata_error();
        return -1;
    }
    while (!(inb(ATA_STATUS) & ATA_STATUS_DRQ));  // Wait for DRQ (data request) bit to be set

    // Read the sector data (512 bytes)
    for (size_t i = 0; i < BYTES_PER_SECTOR / 2; i++) {
        uint16_t data = inw(ATA_DATA);
        ((uint16_t *)buffer)[i] = data;
    }
    return 0;
}

int32_t ata_write_block(uint32_t lba, void *buffer) {
    // Select the master drive
    outb(ATA_DRIVE_SELECT, 0xE0 | ((lba >> 24) & 0x0F));
    // Set the sector count to 1 (write 1 sector)
    outb(ATA_SECTOR_COUNT, 1);
    // Set the LBA (Logical Block Addressing)
    outb(ATA_LBA_LOW, lba & 0xFF);
    outb(ATA_LBA_MID, (lba >> 8) & 0xFF);
    outb(ATA_LBA_HIGH, (lba >> 16) & 0xFF);
    // Send the write command
    outb(ATA_COMMAND, ATA_CMD_WRITE);

    // Wait for the drive to be ready (check BSY and DRQ bits in status register)
    while (inb(ATA_STATUS) & ATA_STATUS_BSY);  // Wait while BSY (busy) bit is set
    if (inb(ATA_STATUS) & ATA_STATUS_ERR) {
        print_ata_error();
        return -1;
    }
    while (!(inb(ATA_STATUS) & ATA_STATUS_DRQ));  // Wait for DRQ (data request) bit to be set

    // Write the sector data (512 bytes)
    for (size_t i = 0; i < BYTES_PER_SECTOR / 2; i++)
        outw(ATA_DATA, ((uint16_t *)buffer)[i]);
    return 0;
}
