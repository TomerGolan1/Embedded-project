#include  "../header/halGPIO.h"     // private library - HAL layer
#include  <stdio.h>
#include  <math.h>

unsigned int adc[2];
double Vx , Vy , angle  ;
int boolStart = 0 , isAdcReady = 0;
char angleString[4];
char modeString[4];
char stepCount[4];
char angle_stepp[5];
int firstTimeScript = 1;
unsigned int deleyX = 32767;
int command = 1;
int mode = 0;
int bool = 0;
int mone_steps = 0 ;
int k=0;
int roundAngle;
char *Flash_ptrB;                          // Flash pointer
unsigned int iB;
char *Flash_ptrC;                          // Flash pointer
unsigned int iC;
char *Flash_ptrD;                          // Flash pointer
unsigned int iD;
int scriptindex;
int RGBcolor = 0x00;
int i;

struct flash_segment{
    int file_number;
    char name_file[3];
    int *ptr_arr[3];
    int size_file[3];
};

struct flash_segment flash_seg;


//--------------------------------------------------------------------
//             System Configuration  
//--------------------------------------------------------------------
void sysConfig(void){
    flash_seg.file_number = 0;
    flash_seg.name_file[0] = "1";
    flash_seg.name_file[1] = "2";
    flash_seg.name_file[2] = "3";
    flash_seg.ptr_arr[0] = 0x1080;
    flash_seg.ptr_arr[1] = 0x1040;
    flash_seg.ptr_arr[2] = 0x1000;
    flash_seg.size_file[0] = 0;
    flash_seg.size_file[1] = 0;
    flash_seg.size_file[2] = 0;
	GPIOconfig();
}


//---------------------------------------------------------------------
//            Polling based Delay function
//---------------------------------------------------------------------
void delay(unsigned int t){  // t[msec]
	volatile unsigned int i;
	
	for(i=t; i>0; i--);
}


//---------------------------------------------------------------------
//            Enter from LPM0 mode
//---------------------------------------------------------------------
void enterLPM(unsigned char LPM_level){
	if (LPM_level == 0x00) 
	  _BIS_SR(LPM0_bits);     /* Enter Low Power Mode 0 */
        else if(LPM_level == 0x01) 
	  _BIS_SR(LPM1_bits);     /* Enter Low Power Mode 1 */
        else if(LPM_level == 0x02) 
	  _BIS_SR(LPM2_bits);     /* Enter Low Power Mode 2 */
	else if(LPM_level == 0x03) 
	  _BIS_SR(LPM3_bits);     /* Enter Low Power Mode 3 */
        else if(LPM_level == 0x04) 
	  _BIS_SR(LPM4_bits);     /* Enter Low Power Mode 4 */
}


//---------------------------------------------------------------------
//            Enable interrupts
//---------------------------------------------------------------------
void enable_interrupts(){
  _BIS_SR(GIE);
}


//---------------------------------------------------------------------
//            Disable interrupts
//---------------------------------------------------------------------
void disable_interrupts(){
  _BIC_SR(GIE);
}


//---------------------------------------------------------------------
//            Timer Wait time with timer A with aclk
//---------------------------------------------------------------------
void TimerWait(int number){
  
  TACCTL0 = CCIE;
  TACCR0 =number;
  TACTL = TASSEL_2 + MC_3 + ID_3 ;   
  
  enable_interrupts();
  enterLPM(lpm_mode);
  TACCTL0 &=~ CCIE;
}


//---------------------------------------------------------------------
//            ADC setup
//---------------------------------------------------------------------

void adc_setup(){

    ADC10SA = (unsigned int) adc;
    ADC10CTL0 |= ENC + ADC10SC ;
}


//*********************************************************************
//            TimerA0 Interrupt Service Rotine
//*********************************************************************       
        
#pragma vector=TIMER0_A0_VECTOR
  __interrupt void timer_a0(void) {
     
  LPM0_EXIT;
}
  

//*********************************************************************
//            UART T Service Rotine
//*********************************************************************

#pragma vector=USCIAB0TX_VECTOR
  __interrupt void USCI0TX_ISR(void){

      if (bool ==0)
      {
          UCA0TXBUF = angleString[k];
          k = k + 1;
          if (UCA0TXBUF == '\n')
            {
                IE2 &= ~UCA0TXIE;
            }
      }
      else if (bool ==1)
      {

          UCA0TXBUF = modeString[k];
          k = k + 1;
          if (UCA0TXBUF == '\n')
          {
              IE2 &= ~UCA0TXIE;
              bool = 0;
          }

      }
      else if(bool ==2)
      {
          UCA0TXBUF = stepCount[k];
          k = k + 1;
          if (UCA0TXBUF == '\n')
          {
              IE2 &= ~UCA0TXIE;
              bool = 0;
          }
       }

      else if(bool ==3)
      {
          UCA0TXBUF = angle_stepp[k];
          k = k + 1;
          if (UCA0TXBUF == '\n')
          {
              IE2 &= ~UCA0TXIE;
              bool = 0;
          }
      }
      else if (bool ==4)
      {
          UCA0TXBUF = '!';
          IE2 &= ~UCA0TXIE;
          bool = 0;
      }
  }


//*********************************************************************
//            UART R Service Rotine
//*********************************************************************

#pragma vector=USCIAB0RX_VECTOR
  __interrupt void USCI0RX_ISR(void){
      if (command ==1)
      {

          ADC10CTL0 &= ~ENC + ~ADC10SC;
          switch (UCA0RXBUF){

          case '1': //first task
              state = state1;
              break;

          case '2': //second task
              state = state2;
              break;

          case '3': //third task
              state = state3;
              break;

          case '4': //fourth task
              state = state4;
              break;

          case '5': //start rotate
              boolStart = 1;
              break;

          case '6': //stop rotate
              boolStart = 0 ;
              break;

          case 'A':  //script 1
              scriptindex = 1;
              break;

          case 'B':  //script 2
              scriptindex = 2;
              break;

          case 'C':  //script 3
              scriptindex = 3;
              break;
          }

          LPM0_EXIT;
      }
      else
      {
          if (scriptindex ==1)
          {
              *Flash_ptrB++ = UCA0RXBUF;
              flash_seg.size_file[0]=flash_seg.size_file[0]+1;
              if (UCA0RXBUF == '!')
              {
                  scriptindex++;
                  flash_seg.file_number=1;
              }
          }
          else if (scriptindex ==2)
          {
              *Flash_ptrC++ = UCA0RXBUF;
              flash_seg.size_file[1]=flash_seg.size_file[1]+1;
              if (UCA0RXBUF == '!')
               {
                     scriptindex++;
                     flash_seg.file_number=2;
               }
          }
          else if (scriptindex ==3)
          {
              *Flash_ptrD++ = UCA0RXBUF;
              flash_seg.size_file[2]=flash_seg.size_file[2]+1;
               if (UCA0RXBUF == '!')
                 {
                       scriptindex++;
                       flash_seg.file_number=3;
                       command = 1;
                       LPM0_EXIT;
                 }
          }

      }

  }


//*********************************************************************
//            ADC10 Interrupt Service Rotine
//*********************************************************************       

#pragma vector=ADC10_VECTOR
  __interrupt void ADC10(void) {

      ADC10CTL0 &= ~ENC + ~ADC10SC;

      Vy = (double) adc[0] ;
      Vx = (double) adc[1] ;
      Vy = Vy - 441 ;
      Vx = Vx - 473 ;

      if (fabs(Vx)<60 && fabs(Vy)< 60)
      {
          angle = 1000 ; //it means the joystick is on the midddle (angle cannot be 1000)
      }
      else
      {
          angle = atan2(Vy , Vx);
          angle = (angle /3.1415) *180 ;
          angle = angle + 180 ;         //that's for getting angle between [0, 360]

      }
      isAdcReady = 1 ;
      ADC10CTL0 |= ENC + ADC10SC;
      LPM0_EXIT;

}

//*********************************************************************
//            Port1 Interrupt Service Rotine
//*********************************************************************
#pragma vector=PORT1_VECTOR
  __interrupt void PBs_handler(void){
   
      delay(debounceVal);
      delay(debounceVal);

//---------------------------------------------------------------------
//            selector of transition between states
//---------------------------------------------------------------------
	if(PBsArrIntPend1 & PB0){
	    PBsArrIntPend1 &= ~PB0;
	    mode = (mode +1)%3;
	    bool =1;
        modeString[2]= (char) (0 +48) ;
        modeString[1] = (char) ((0 +48 )) ;
        modeString[0] = (char) ((5 +48)) ;
        modeString[3] = '\n';
        k = 0 ;
        IE2 |= UCA0TXIE;
        TimerWait(3250);
        }
}
