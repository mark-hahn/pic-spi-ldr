
#ifndef SPI_H
#define	SPI_H

#define SPI_ENBL          SSP1CON1bits.SSPEN // SPI enable bit
#define SPI_OVFLW         SSP1CON1bits.SSPOV  // spi overflow flag
#define SPI_WCOL          SSP1CON1bits.WCOL   // spi write collision flag
#define SPI_DATA_BUF      SSP1BUF // SPI data byte buffer, 8 bits
#define SPI_DATA_IF       SSP1IF  // SPI data byte buffer filled int flag
#define SPI_SS_IOC        IOCAPbits.IOCAP7    // interrupt on rising A7 pin
#define SPI_SS_IOC_IF     IOCAFbits.IOCAF7    // interrupt flag

// these packets have all bytes in one SS low period
// bottom nibble zero in case CPU sends while not flashing
#define ERASE_CMD  0x10  // cmd byte + 2-byte word address
#define WRITE_CMD  0x20  // cmd byte + 2-byte word address and 32 words(64 bytes)
#define RESET_CMD  0x30  // cmd byte only, reset processor, issue when finished

#define STATUS_FLASHING 7  // must match mcu-cpu.h, i.e. don't change

void spiInit();
void chkSpi();

#endif	/* SPI_H */

