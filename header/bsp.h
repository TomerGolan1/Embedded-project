#ifndef _bsp_H_
#define _bsp_H_

#include  <msp430g2553.h>          // MSP430x2xx

#define   debounceVal      250
#define   LEDs_SHOW_RATE   0xFFFF  // 62_5ms

// LEDs abstraction
#define LEDsArrPort        P1OUT
#define LEDsArrPortDir     P1DIR
#define LEDsArrPortSel     P1SEL

// Switches abstraction
#define SWsArrPort         P2IN
#define SWsArrPortDir      P2DIR
#define SWsArrPortSel      P2SEL
#define SWmask             0x0F

// PushButtons abstraction
#define PBsArrPort2	       P2IN
#define PBsArrPort1	       P1IN
#define PBsArrIntPend2	   P2IFG
#define PBsArrIntPend1	   P1IFG
#define PBsArrIntEn2	   P2IE
#define PBsArrIntEn1	   P1IE
#define PBsArrIntEdgeSel2  P2IES
#define PBsArrIntEdgeSel1  P1IES
#define PBsArrPortSel2     P2SEL 
#define PBsArrPortDir2     P2DIR

#define PB0                0x01
#define PB1                0x02
#define PB2                0x04
#define PB3                0x08
#define PB5                0x10

#define PBOUT              P2OUT

#define TXLED              BIT0
#define RXLED              BIT6
#define TXD                BIT2
#define RXD                BIT1


extern void GPIOconfig(void);
extern void ADCconfig(void);

#endif



