//Global Constants/////////////////
const int xpin = A0;
const int ypin = A2;
const int zpin = A4;
//////////////////////////////////

//Macros///////////////////////////////////////////////////
#define Baude 9600//Define serial board baude rate 
#define SampleSize 10// Define Accelerometer sample size 
#define ArraySize 4//define data array size 
///////////////////////////////////////////////////////////

//Setup function//////////////////////////////////////////////////////////////////////
void setup() {
Serial.begin(Baude);//serial communication at 9600b/s
 Serial.print("$"); 
 Serial.print("$");
 Serial.print("$");// Enter command mode on bluetooth module 
 delay(100);  // Short delay, wait for the bluetooth module to send back CMD
 Serial.println("C,0006667C8A90");  //  command to connect to slave bluetooth module 
}
////////////////////////////////////////////////////////////////////////////////////////


//Main/////////////////////////////////////////////////////////////////////////////////
void loop() {
  unsigned int i=0; //counter number to acceleration reads 
  unsigned int xsum,ysum,zsum=0; //hold accelerometer accces reads 
  unsigned char xzdata[ArraySize];//array to hold data that will be sent to HSC12      
  
  for (i = 0; i < SampleSize; i++) {    //colect ten readings from each axis 

    xsum+= analogRead(xpin);

    ysum+= analogRead(ypin);

    zsum+= analogRead(zpin);
    delay(1);
  }
  
  xsum= (xsum/SampleSize);//average each axis reading 
  ysum= (ysum/SampleSize);
  zsum= (zsum/SampleSize);
  
 xzdata[0]= char(xsum>>8);//x axis MSB
 xzdata[1]= char(xsum);//x axis LSB
 xzdata[2]= char(zsum>>8);//z axis MSB
 xzdata[3]= char(zsum);//z axis LSB

 Serial.write(xzdata,ArraySize);//sends each element of xzdata array to HCS12 
}
////////////////////////////////////////////////////////////////////////////////////////////
