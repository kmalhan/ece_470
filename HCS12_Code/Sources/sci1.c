#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "sci1.h"   /* sci1 interface header file*/

#define CR (0x0D)
#define LF (0x0A)

///////////////////////////////////////////////////////////////////////
void SCI1_init(long eclock, long baud_rate){
     int br = (int)(  eclock/(16 * baud_rate) );
     SCI1BDL =  (char)br ;          //put LSB
     SCI1BDH =   (char)(br >> 8) ; //put MSB 
     SCI1CR1 = 0;      //8N1
     SCI1CR2 = 0x2C;   //enable TX and RE, and enable Recv Interrupt
}
//////////////////////////////////////////////////////////////////////
void  SCI1_putchar(char c){
    
    while(   (SCI1SR1 & 0x80 ) == 0 )   //wait until TDRE becomes 1
      ;
        
    SCI1DRL= c;
}
////////////////////////////////////////////////////////////////////////
void SCI1_print(char *s){
    while(*s)
       SCI1_putchar(*s++); 
    
}
//////////////////////////////////////////////////////////////////////
void SCI1_println(char *s){
  SCI1_print(s);
  SCI1_putchar(CR);
  SCI1_putchar(LF);  
}
/////////////////////////////////////////////////////////////////////
unsigned char SCI1_getchar(void){
     
     while((SCI1SR1 & (1<<5))  == 0)  //wait until you receive char
       ; 
     
     return SCI1DRL;
}

