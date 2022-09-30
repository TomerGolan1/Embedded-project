#include  "../header/api.h"    		// private library - API layer
#include  "../header/app.h"    		// private library - APP layer

enum FSMstate state;
enum SYSmode lpm_mode;

void main(void){

  state = state0;       
  lpm_mode = mode0;     
  sysConfig();


  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT

    if (CALBC1_1MHZ==0xFF)                    // If calibration constant erased
    {
      while(1);
    }
    DCOCTL = 0;                               // Select lowest DCOx and MODx settings
    BCSCTL1 = CALBC1_1MHZ;                    // Set DCO
    DCOCTL = CALDCO_1MHZ;


  while(1){
	switch(state){
	  case state0:
	      enable_interrupts();
	      enterLPM(lpm_mode);
		break;
		 
	  case state1:
	      stepper_motor();
		break;
		 
	  case state2:
	      paint();
		break;
                
	  case state3:
	     calibrate();
         break;
                
	  case state4:
	      script();
	     break;
	}
  }
}
  
  
  
  
  
  
