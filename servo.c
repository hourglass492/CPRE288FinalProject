/*
 * servo.c
 *
 *  Created on: Apr 1, 2020
 *      Author: Adamson, Bryanna M
 */

#include <inc/tm4c123gh6pm.h>
#include "open_interface.h"


unsigned int scaler;
int offset;
unsigned int matchVal;

void servo_init(){
  //Enable clock for port b
   SYSCTL_RCGCGPIO_R |= 0x02;
   //Enable alternate function
   GPIO_PORTB_AFSEL_R |= 0x20;
   //Select alternate function
   GPIO_PORTB_PCTL_R |= 0x00700000;
   //Enable PB5
   GPIO_PORTB_DEN_R |= 0x20;
   //Set PB5 as an output
   GPIO_PORTB_DIR_R |= 0x20;
   //Enable clock for TIMER 1B
   SYSCTL_RCGCTIMER_R |= SYSCTL_RCGCTIMER_R1;
   //Disable the TIMER for config
   TIMER1_CTL_R &= ~TIMER_CTL_TBEN;
   //Select TIMER configuration
   TIMER1_CFG_R |= 0x04;
   //Set TIMER mode, PWM mode with periodic count
   TIMER1_TBMR_R |= 0xA;
   // start value in clock cycles (ms to clock cycles)
   unsigned int pwm_period;
   pwm_period = 20 * 1000 * 16;

   // lower 16 bits of start value
   TIMER1_TBILR_R = pwm_period & 0xFFFF; //This register can be completly over written
   // next 8 bits of the start value
   TIMER1_TBPR_R = pwm_period >> 16;


   //match value
   TIMER1_TBMATCHR_R = (195 * 100 * 16);
   TIMER1_TBPMR_R = (195 * 100 * 16) >> 16;



   //Enable the TIMER
   TIMER1_CTL_R |= TIMER_CTL_TBEN;


   offset = 0;
   scaler = 160;


}




void servo_set_angle(int angle){

    //TODO Need to make the angle changeable


    //Data sheet says we can convert between angle and time with mx+b.
    // This lets us change the values in the code
    matchVal= scaler * angle + offset;

    TIMER1_TBMATCHR_R = (matchVal);
    TIMER1_TBPMR_R = (matchVal) >> 16;

}

void servo_set_match_val(unsigned int val){
    matchVal = val;
}


void servo_set_scalling_for_function(int m){
    scaler = m;
}

void servo_set_offset_for_function(int b){
    offset = b;
}

unsigned int servo_get_match_val(){
    return matchVal;
}

unsigned int servo_get_scaleing(){
    return scaler;
}

int servo_get_offset(){
    return offset;
}


