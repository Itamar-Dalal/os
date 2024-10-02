#ifndef ATA_H
#define ATA_H

void ata_read_block(uint32_t lba, void *buffer);
void ata_write_block(uint32_t lba, void *buffer);

#endif