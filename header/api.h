#ifndef _api_H_
#define _api_H_

#include  "../header/halGPIO.h"     // private library - HAL layer

extern void stepperSpin(int stepNum, int spinDirection);
extern void runScript(int scriptnumber);
extern void blink_rgb(int x);
extern void rrc_leds(int x);
extern void rlc_leds(int x);
extern void stepper_deg1(int x, char ch);
extern void stepper_deg(int x);
extern void stepper_scan(int l, int r);
extern void set_delay(int d);
extern void clear_all_leds(void);
extern int hex_to_int(char hex_a, char hex_b);

extern void calibrate();
extern void stepper_motor();
extern void paint();
extern void script();


#endif







