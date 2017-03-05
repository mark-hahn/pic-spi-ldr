
#include <xc.h>
#include "spi.h"
#include "pins-b.h"
#include "main.h"
#include "flash.h"

char         cmd;
unsigned int wordAddr;
char         buf[MAX_BYTES_IN_BLOCK];
#define BUF_START_IDX -3
signed char  packetByteIdx;  // -3: cmd, -2..-1: wordAddress, 0..: data

void spiInit() {
  SSP1SSPPS  = SPI_SS_PPS;       // A7 => slave select in
  SSP1CLKPPS = SPI_CLK_PPS;      // C3 => clock in
  SSP1DATPPS = SPI_DATA_IN_PPS;  // C4 => data in
  SPI_DATA_OUT_PPS = 0x16;       // Data out => C5
   
  SPI_SS_TRIS       = 1;         // SPI TRIS select input
  SPI_CLK_TRIS      = 1;         // SPI TRIS clock input
  SPI_DATA_IN_TRIS  = 1;         // SPI TRIS data input
  SPI_DATA_OUT_TRIS = 0;         // SPI TRIS data output
  
  SSP1CON1bits.SSPM  = 4; // mode: spi slave with slave select enabled
  SSP1STATbits.SMP   = 0; // input sample edge (must be zero for slave mode)
  SSP1CON1bits.CKP   = 0; // 0: xmit clk low is idle
  SSP1STATbits.CKE   = 1; // clk edge (1: active ->idle) (1: safe wcol?)
  SSP1CON3bits.BOEN  = 0; // disable buffer input overflow check (SSPOV))
  /* From datasheet: Before enabling the module in SPI Slave mode, the clock
   line must match the proper Idle state (CKP) */
//  while(SPI_CLK);
  
  SPI_ENBL = 1; // enable SPI
  // start on word boundary
//  while(!SPI_SS);
  
  SPI_OVFLW     = 0; // clear errors
  SPI_WCOL      = 0;
  SPI_SS_IOC    = 1; // SS rising IOC
  SPI_DATA_IF   = 0; // clear int flags
  SPI_SS_IOC_IF = 0; // IOC int flag for specific A7 SS pin
  
  packetByteIdx = BUF_START_IDX;
}

void chkSpi() {
  if(SPI_DATA_IF) {
    // SPI data byte received
    char byteIn = SPI_DATA_BUF;
    SPI_DATA_IF = 0;  
    SPI_WCOL = 0; // clear error
    if(packetByteIdx <  MAX_BYTES_IN_BLOCK) {
      switch(packetByteIdx) {
        case -3: cmd = byteIn;                        break;
        case -2: wordAddr = byteIn;                   break;
        case -1: wordAddr = (wordAddr << 8) | byteIn; break;
        default: buf[packetByteIdx] = byteIn;         break;
      }
    }
    packetByteIdx++;
  }
  if(SPI_SS_IOC_IF) {
    // SS just went high -- packet finished
    switch (cmd) {
      case WRITE_CMD: flash_memory_write(wordAddr, buf); break;
      case ERASE_CMD: flash_memory_erase(wordAddr);      break;
      case RESET_CMD: 
        for(char i=0; i < 6; i++) FAN_LAT = !FAN_LAT;
        RESET(); // goodbye cruel world
    }
    // cpu may have just resumed after flash delay froze it
    // make sure we are in sync with data
    while(!SPI_SS);
    SPI_DATA_BUF  = STATUS_FLASHING; 
    packetByteIdx = BUF_START_IDX;
    SPI_SS_IOC_IF = 0;
  }
}

