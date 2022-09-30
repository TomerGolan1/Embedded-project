#ifndef _halGPIO_H_
#define _halGPIO_H_

#include  "../header/bsp.h"    		// private library - BSP layer
#include  "../header/app.h"    		// private library - APP layer

extern enum FSMstate state;   // global variable
extern enum SYSmode lpm_mode; // global variable

extern void sysConfig(void);
extern void delay(unsigned int);
extern void enterLPM(unsigned char);
extern void enable_interrupts();
extern void disable_interrupts();
extern void adc_setup();
extern void TimerWait(int number);

extern __interrupt void PBs_handler(void);
extern __interrupt void USCI0RX_ISR(void);
extern __interrupt void USCI0TX_ISR(void);
#endif

extern unsigned int adc[2];
extern double Vx , Vy , angle  ;
extern int boolStart , isAdcReady;
extern char modeString[4];
extern char stepCount[4];
extern char angleString[4];
extern char angle_stepp[5];
extern int firstTimeScript;
extern unsigned int deleyX;
extern int command;

extern int mode;
extern int bool;
extern int mone_steps ;

extern int k;
extern int roundAngle;
extern char *Flash_ptrB;                          // Flash pointer
extern unsigned int iB;
extern char *Flash_ptrC;                          // Flash pointer
extern unsigned int iC;
extern char *Flash_ptrD;                          // Flash pointer
extern unsigned int iD;
extern int scriptindex;

extern int RGBcolor;

extern int i;

extern struct flash_segment flash_seg;






