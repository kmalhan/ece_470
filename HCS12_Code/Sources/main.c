/* Final Prject Code (HCS12 side) *****
*                                                             *
*    Kazumi Malhan                               *
*    Justen Beffa                                     *
*                                                             *
*************************************/

/* Note ***********************************
*
*       SCI1 Pin Connection: 
*                   PT2: Rx
*                   PT3: Tx
*       
*        Toy Control (PORTB):
*                    Bit 0: Forward
*                    Bit 2: Backward
*                    Bit 4: Left
*                    Bit 6: Right
*
*******************************************/


/*****************************************************************************
*
*  Include Files
*
*****************************************************************************/
#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "sci1.h"
#include "led.h"
#include "delay.h"
#include "sci0.h"

/*****************************************************************************
*
*  Macro Definitions
*
*****************************************************************************/
#define E_CLOCK (4000000)   
#define BR (9600)   
#define CR (0x0D)
#define LF (0x0A)

#define SCI_DATA_SIZE   4
#define MSB_X   0
#define LSB_X    1
#define MSB_Z   2
#define LSB_Z    3

// Output control
#define STOP                  (0b00000000)
#define FORWARD       (0b00000001)
#define BACKWARD    (0b00000100)
#define LEFT                  (0b00010000)
#define RIGHT               (0b01000000)
#define FORWARD_LEFT   (FORWARD | LEFT)
#define FORWARD_RIGHT   (FORWARD | RIGHT)
#define BACKWARD_LEFT   (BACKWARD | LEFT)
#define BACKWARD_RIGHT  (BACKWARD | RIGHT)

// Threshold value
#define F_TH_X  360              //335   // above
#define B_TH_X  300            //300   // below
#define L_TH_Z   370           //375   // above
#define R_TH_Z   310          //310   // below

/*****************************************************************************
*
*  Function Prototype
*
*****************************************************************************/
interrupt 21 void SCI1_ISR(void);
void Process_Data(void);                    // Main task to process data

/*****************************************************************************
*
*  Global Variables
*
*****************************************************************************/
volatile unsigned char accReading[SCI_DATA_SIZE];               // Raw reading of acceleration
volatile unsigned char accIndex;                                                   // Current index of accReading
volatile unsigned char useReading[SCI_DATA_SIZE];               // Reading of acceleration used by main process

volatile unsigned char performTask;             // Flag to start processing data
volatile unsigned char taskDone;                   // Indicate that task is done

/*****************************************************************************
*
*  ----- Main ------
*
*****************************************************************************/
void main(void) {
   
   SCI1_init(E_CLOCK,BR );
   SCI0_init(E_CLOCK,BR );
   LEDs_init();
   accIndex = 0;
   performTask = 0;
   taskDone = 1;
   
   DDRA = 0xFF;
   PORTA = 0;
   
   asm{
    CLI
   }
   
   for(;;){
   
       if (performTask){
            performTask = 0;
            Process_Data();
       }
   }//for  
  
}//main

/*****************************************************************************
*
*  SCI1 Interrupt
*
*****************************************************************************/
interrupt 21 void SCI1_ISR(void){
    if ((SCI1SR1 & SCI1SR1_RDRF_MASK) ) {
        unsigned char i;
     
        accReading[accIndex] = SCI1DRL;  
        
        if (accIndex == (SCI_DATA_SIZE - 1) ){  // when filled all element in array
            accIndex = 0;                                           // reset accIndex
            if (taskDone){                                          // if task is done, update useReading, put flag
                taskDone = 0;
                for (i = 0; i < (SCI_DATA_SIZE); i++){
                    useReading[i] = accReading[i];
                }
                performTask = 1; 
            }  
        } else {
            accIndex++;
        }
    }
}

/*****************************************************************************
*
*  Process_Data();
*
*****************************************************************************/
void Process_Data(){
       unsigned int x = 0;
       unsigned int z = 0; 
       
       // Process x
       x = useReading[MSB_X];
       x <<= 8;
       x |= useReading[LSB_X];
       
       SCI0_printInt(x);
       SCI0_print(" ");
       
       // Process z
       z = useReading[MSB_Z];
       z <<= 8;        
       z |= useReading[LSB_Z];
       
       SCI0_printInt(z);
       SCI0_println(" ");
       
       // Evaluate and output 
       if (x > F_TH_X){
            if (z > L_TH_Z){
                PORTB = FORWARD_LEFT;
                PORTA = FORWARD_LEFT;
            } else if (z < R_TH_Z){
                PORTB = FORWARD_RIGHT;
                PORTA = FORWARD_RIGHT;
            } else {
                PORTB = FORWARD;
                PORTA = FORWARD;
            }
       } else if (x < B_TH_X) {
             if (z > L_TH_Z){
                PORTB = BACKWARD_LEFT;
                PORTA = BACKWARD_LEFT;
            } else if (z < R_TH_Z){
                PORTB = BACKWARD_RIGHT;
                PORTA = BACKWARD_RIGHT;
            } else {
                PORTB = BACKWARD;
                PORTA = BACKWARD;
            }
       } else {
            if (z > L_TH_Z){
                PORTB = LEFT;
                PORTA = LEFT;
            } else if (z < R_TH_Z){
                PORTB = RIGHT;
                PORTA = RIGHT;
            } else {
                PORTB = STOP;
                PORTA = STOP;
            }
       } 
       taskDone = 1; 
}