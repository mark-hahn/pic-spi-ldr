/* 
 * File:   flash.h
 * Author: root
 *
 * Created on January 28, 2017, 10:03 PM
 */

#ifndef FLASH_H
#define	FLASH_H

#define WRITE_FLASH_BLOCKSIZE 32  // memory words, 14-bit
#define ERASE_FLASH_BLOCKSIZE 32
#define MAX_BYTES_IN_BLOCK    64
#define END_FLASH 0x0220


char haveApp();
//unsigned int flash_memory_read (unsigned int address); // not used
void flash_memory_erase (unsigned int wordAddress);
void flash_memory_write (unsigned int wordAddress, unsigned char *data);

#define uint8_t unsigned char
#define int8_t signed char
#define uint16_t unsigned int

int8_t FLASH_WriteBlock(uint16_t writeAddr, uint16_t *flashWordArray);


#endif	/* FLASH_H */

