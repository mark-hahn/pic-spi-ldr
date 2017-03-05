
#ifndef PINS_B_H
#define	PINS_B_H

// MCU pin assignments for XY rev B board

// TRIS, tri-state selects
#define SPI_SS_TRIS       TRISA7 // SPI Slave Select input, SS
#define SPI_CLK_TRIS      TRISC3 // SPI Clock input, SCLK
#define SPI_DATA_IN_TRIS  TRISC4 // SPI Data input,  MOSI
#define SPI_DATA_OUT_TRIS TRISC5 // SPI Data output, MISO

// pin input values unlatched
#define SPI_SS            RA7    // SPI Slave Select level, no latch
#define SPI_CLK           RC3    

// SPI: PPS, peripheral select
#define SPI_SS_PPS        0x07    // SSP1SSPPS  <= A7, SPI Slave Select input
#define SPI_CLK_PPS       0x13    // SSP1CLKPPS <= C3, SPI Clock input
#define SPI_DATA_IN_PPS   0x14    // SSP1DATPPS <= C4, SPI Data input
#define SPI_DATA_OUT_PPS  RC5PPS  // Data out   => C5, SPI Data output

#define FAN_TRIS          TRISC1 // fan pin output
#define FAN_LAT           LATC1  // fan pin latch, for debug ouput

#endif	/* PINS_B_H */

