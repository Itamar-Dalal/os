#ifndef ATA_H
#define ATA_H

void ata_read_block(uint32_t lba, uint8_t *buffer);
void ata_write_block(uint32_t lba, uint8_t *buffer);

#endif