#ifndef ATA_H
#define ATA_H

#define BYTES_PER_SECTOR 512

int32_t ata_read_block(uint32_t lba, void *buffer);
int32_t ata_write_block(uint32_t lba, void *buffer);

#endif