#ifndef ATA_H
#define ATA_H

int32_t ata_read_block(uint32_t lba, void *buffer);
int32_t ata_write_block(uint32_t lba, void *buffer);

#endif