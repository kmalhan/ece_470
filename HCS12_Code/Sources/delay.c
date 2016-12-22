/* Delay implementation */

#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "delay.h"

void delay_us(unsigned int us){
  unsigned int start, ticks;
  
  ticks  = (unsigned)(us * 4UL / 32);
  TSCR1 = 0x80;
  TSCR2 = 0x05;  //tick at 24Mhz/32 = 750 kHz
  start = TCNT;
  
  while( (TCNT - start)<= ticks)
    ;
}

void delay_ms(unsigned int ms){
  while( ms--) delay_us(1000);
}