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

#define ATA_CMD_READ  0x20       // Read command
#define ATA_CMD_WRITE 0x30       // Write command

void ata_read_block(uint32_t lba, void *buffer) {
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
    while (inb(ATA_STATUS) & 0x80);  // Wait while BSY (busy) bit is set
    while (!(inb(ATA_STATUS) & 0x08));  // Wait for DRQ (data request) bit to be set
    // Read the sector data (512 bytes)
    for (int i = 0; i < 256; i++) {
        uint16_t data = inw(ATA_DATA);
        ((uint16_t *)buffer)[i] = data;
    }
}

void ata_write_block(uint32_t lba, uint8_t *buffer) {
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
    while (inb(ATA_STATUS) & 0x80);  // Wait while BSY (busy) bit is set
    while (!(inb(ATA_STATUS) & 0x08));  // Wait for DRQ (data request) bit to be set
    // Write the sector data (512 bytes)
    for (int i = 0; i < 256; i++) {
        outw(ATA_DATA, ((uint16_t *)buffer)[i]);
    }
}
