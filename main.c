
// SPI bootloader for PIC16LF15355

// code used with the bootloader should offset the code by 0x200
// search xc8 user manual for --CODEOFFSET and --ROM

#pragma config FEXTOSC = OFF      // External Oscillator mode selection bits (Oscillator not enabled)
#pragma config RSTOSC = HFINT32   // Power-up default value for COSC bits (HFINTOSC with OSCFRQ= 32 MHz and CDIV = 1:1)
#pragma config CLKOUTEN = OFF     // Clock Out Enable bit (CLKOUT function is disabled; i/o or oscillator function on OSC2)
#pragma config CSWEN = ON         // Clock Switch Enable bit (Writing to NOSC and NDIV is allowed)
#pragma config FCMEN = ON         // Fail-Safe Clock Monitor Enable bit (FSCM timer enabled)
#pragma config MCLRE = ON         // MCLR pin is a Clear function, must be ON for debugger
#pragma config PWRTE = OFF        // Power-up Timer Enable bit (PWRT disabled)
#pragma config LPBOREN = OFF      // Low-Power BOR enable bit (ULPBOR disabled)
#pragma config BOREN = ON         // Brown-out reset enable bits (Brown-out Reset Enabled, SBOREN bit is ignored)
#pragma config BORV = LO          // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (VBOR) set to 1.9V on LF, and 2.45V on F Devices)
#pragma config ZCD = OFF          // Zero-cross detect disable (Zero-cross detect circuit is disabled at POR.)
#pragma config PPS1WAY = ON       // Peripheral Pin Select one-way control (The PPSLOCK bit can be cleared and set only once in software)
#pragma config STVREN = ON        // Stack Overflow/Underflow Reset Enable bit (Stack Overflow or Underflow will cause a reset)
#pragma config WDTCPS = WDTCPS_31 // WDT Period Select bits (Divider ratio 1:0x10000; software control of WDTPS)
#pragma config WDTE = OFF         // WDT operating mode (WDT enabled regardless of sleep; SWDTEN ignored)
#pragma config WDTCWS = WDTCWS_7  // WDT Window Select bits (window always open (100%); software control; keyed access not required)
#pragma config WDTCCS = SC        // WDT input clock selector (Software Control)
#pragma config BBSIZE = BB512     //  (512 char boot block size)
#pragma config BBEN = ON          //  (Boot Block enabled)
#pragma config SAFEN = OFF        //  (SAF disabled)
#pragma config WRTAPP = OFF       //  (Application not write protected)
#pragma config WRTB = OFF         //  (Boot Block not write protected)
#pragma config WRTC = OFF         //  (Configuration Register not write protected)
#pragma config WRTSAF = OFF       //  (SAF not write protected)
#pragma config LVP = OFF          // Low Voltage Programming Enable bit (High Voltage on MCLR/Vpp must be used for programming)
#pragma config CP = OFF           // UserNVM Program memory code protection bit (UserNVM code protection disabled)

#include <xc.h>
#include "main.h"
#include "pins-b.h"
#include "spi.h"
#include "flash.h"

#define _str(x)  #x
#define  str(x)  _str(x)

void interrupt main_isr() { // redirect 0x0004 int to 0x0204
  asm ("pagesel  " str (NEW_INTERRUPT_VECTOR));
  asm ("goto   "   str (NEW_INTERRUPT_VECTOR));
}     

void main(void) {
  INTCON = 0;  // turn off all interrupts

  ANSELA = 0; // no analog inputs
  ANSELB = 0; // these &^%$&^ regs cause a lot of trouble
  ANSELC = 0; // they should not default to on and override everything else

  FAN_TRIS = 0;  // debug
  FAN_LAT = 0;
  
  // after first app load, to run bootloader the app should erase 0x200
  if(haveApp()) {   // jump to app
//    for(char i=0; i < 6; i++) FAN_LAT = !FAN_LAT;
    STKPTR = 0x1F;
    asm ("pagesel " str(NEW_RESET_VECTOR));
    asm ("goto  "  str(NEW_RESET_VECTOR));
  }
  // run bootloader
//  for(char i=0; i < 4; i++) FAN_LAT = !FAN_LAT;
  spiInit();
  while(1) chkSpi();
}
