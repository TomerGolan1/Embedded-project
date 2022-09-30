#include  "../header/bsp.h"    // private library - BSP layer

//-----------------------------------------------------------------------------  
//           GPIO congiguration
//-----------------------------------------------------------------------------

void GPIOconfig(void){
 // volatile unsigned int i; // in case of while loop usage
  
  WDTCTL = WDTHOLD | WDTPW;		// Stop WDT
   
  // PushButtons Setup - port 2
  PBsArrPortSel2 &= ~0xFF;
  PBsArrPortDir2 |= 0xFF;
  PBsArrIntEdgeSel2 |= 0x01;         // pull-down mode
  PBsArrIntEn2 &= ~ 0xFF;
  PBsArrIntPend2 &= ~0xFF;  
  PBOUT = 0x00;
  
  // PushButtons Setup - port 1
  PBsArrIntEn1 &= ~ 0xFF;
  PBsArrIntEn1 |= 0x01;
  PBsArrIntPend1 &= ~0xFF;            // clear pending interrupts 
  LEDsArrPortSel &= ~0xFF;            // GPIO capability
  LEDsArrPortDir |= 0xE0;


 //UART Config
  P1SEL |= BIT1 + BIT2 ;                   // P1.1 = RXD, P1.2=TXD
  P1SEL2 |= BIT1 + BIT2 ;
  P1DIR |= RXLED + TXLED;
  P1OUT &=~ (BIT1 + BIT2);

  UCA0CTL1 |= UCSSEL_2;                     // CLK = SMCLK
  UCA0BR0 = 104;
  UCA0BR1 = 0x00;
  UCA0MCTL = UCBRS0;
  UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
  IE2 |= UCA0RXIE;                          // Enable USCI_A0 RX interrupt

  P1SEL &=~ 0x01;                       //joystick push button config
  P1DIR &=~ 0x01;
  P1IE |= 0x01;
  PBsArrIntEdgeSel1 |= 0x01;  // pull-up mode new

  _BIS_SR(GIE);               // enable interrupts globally
}                             


//------------------------------------------------------------------------------------- 
//            ADC congiguration 
//-------------------------------------------------------------------------------------
void ADCconfig(void){
    P1SEL &= ~(BIT3 + BIT4);
    P1DIR &= ~(BIT3 + BIT4);
    P1SEL2 &= ~(BIT3 + BIT4);

    ADC10CTL1 = INCH_4 + CONSEQ_1 + SHS_0 + ADC10SSEL_0 ; //  Repeat signal channel , ADC10SC

    ADC10AE0 = BIT3 + BIT4;  //0x1F                        // P1.3-4 ADC option select
    ADC10DTC1 = 5;                         // 5 conversions
    ADC10CTL0 = ADC10SHT_2 + MSC + ADC10ON + ADC10IE + SREF_0;
}              

             

