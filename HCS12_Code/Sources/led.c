/* LED Interface Implementation */

#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "led.h"

void LEDs_init(){
   DDRB =0xFF;
   PORTB=0; 
}

void LED_on(int led_num){
    PORTB |= 1<<led_num;  
}

void LED_off(int led_num){
    PORTB &= ~(1<<led_num);  
}

void LED_toggle(int led_num){
    PORTB ^= 1 << led_num;  
}