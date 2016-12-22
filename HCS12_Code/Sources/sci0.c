#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "sci1.h"   /* sci1 interface header file*/

#define CR (0x0D)
#define LF (0x0A)

///////////////////////////////////////////////////////////////////////
void SCI0_init(long eclock, long baud_rate){
     int br = (int)(  eclock/(16 * baud_rate) );
     SCI0BDL =  (char)br ;          //put LSB
     SCI0BDH =   (char)(br >> 8) ; //put MSB 
     SCI0CR1 = 0;      //8N1
     SCI0CR2 = 0x0C;   //enable TX and RE   
}
//////////////////////////////////////////////////////////////////////
void  SCI0_putchar(char c){
    
    while(   (SCI0SR1 & 0x80 ) == 0 )   //wait until TDRE becomes 1
      ;
        
    SCI0DRL= c;
}
////////////////////////////////////////////////////////////////////////
void SCI0_print(char *s){
    while(*s)
       SCI0_putchar(*s++); 
    
}
//////////////////////////////////////////////////////////////////////
void SCI0_println(char *s){
  SCI0_print(s);
  SCI0_putchar(CR);
  SCI0_putchar(LF);  
}
/////////////////////////////////////////////////////////////////////
unsigned char SCI0_getchar(void){
     
     while((SCI0SR1 & (1<<5))  == 0)  //wait until you receive char
       ; 
     
     return SCI0DRL;
}

void reverse(char *s){
   int i = 0, j = 0;
   
   while(s[j]) 
     j++;
   j -= 1;
   
   while( i < j ){
       char tmp = s[j];
       s[j]= s[i];
       s[i] = tmp;
       i++;
       j--;
   }    
}

void int2str(unsigned int n, char s[]){
     int i;
     
     i = 0;
     do{      
       s[i++] = n %10 + 0x30 ;             
     }while(n/=10);
     
     s[i] = 0 ;
     
     reverse(s);
  
}
void SCI0_printInt(unsigned int n){
   char a[20];
   int2str(n, a);
   SCI0_print(a);
}

