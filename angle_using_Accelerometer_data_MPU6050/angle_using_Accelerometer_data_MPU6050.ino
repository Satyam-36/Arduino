#include <Wire.h>
#include<math.h>
const int MPU = 0x68; // MPU6050 I2C address
float AccX, AccY, AccZ;
float angleX,angleY;
int c=200;
float angleErrorX=0,angleErrorY=0;
void setup() {
  Serial.begin(9600);
  Wire.begin();                      // Initialize comunication
  Wire.beginTransmission(MPU);       // Start communication with MPU6050 // MPU=0x68
  Wire.write(0x6B);                  // Talk to the register 6B
  Wire.write(0x00);                  // Make reset - place a 0 into the 6B register
  Wire.endTransmission(true);        //end the transmission
  delay(20);
  readAccelError();
}
void loop() {
  readAccel();
  Serial.print(" x ");
  Serial.print(angleX);
  Serial.print(" | ");
  Serial.print(" y ");
  Serial.println(angleY);
}

void readAccel()
{
  // === Read acceleromter data === //

    Wire.begin();                      // Initialize comunication
  Wire.beginTransmission(MPU);       // Start communication with MPU6050 // MPU=0x68
  Wire.write(0x6B);                  // Talk to the register 6B
  Wire.write(0x00);                  // Make reset - place a 0 into the 6B register
  Wire.endTransmission(true);        //end the transmission
 
  // Configure Accelerometer Sensitivity - Full Scale Range (default +/- 2g)
  Wire.beginTransmission(MPU);
  Wire.write(0x1C);                  //Talk to the ACCEL_CONFIG register (1C hex)
  Wire.write(0x10);                  //Set the register bits as 00010000 (+/- 8g full scale range)
  Wire.endTransmission(true);
  
 //Accessing Register to activate low pass filter
        Wire.beginTransmission(MPU);
        Wire.write(0x1A);
        Wire.write(0x05); 
        Wire.endTransmission();
        
  Wire.beginTransmission(MPU);
  Wire.write(0x3B); // Start with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 6, true); // Read 6 registers total, each axis value is stored in 2 registers
  //For a range of +-2g, we need to divide the raw values by 16384, according to the datasheet
  AccX = (Wire.read() << 8 | Wire.read()) / 4096.0; // X-axis value
  AccY = (Wire.read() << 8 | Wire.read()) / 4096.0; // Y-axis value
  AccZ = (Wire.read() << 8 | Wire.read()) / 4096.0; // Z-axis value

  AccX=AccX/0.97;
  AccY=AccY/0.97;
  AccZ=AccZ/0.97;

  if(AccX>1)
  {
    AccX=1;
  }
   if(AccX<-1)
  {
    AccX=-1;
  }

  //////////////////////////////

   if(AccY>1)
  {
    AccY=1;
  }
   if(AccY<-1)
  {
    AccY=-1;
  }
  ////////////////////////////////
   if(AccZ>1)
  {
    AccZ=1;
  }
   if(AccZ<-1)
  {
    AccZ=-1;
  }
 angleX=(atan(AccY/sqrt(pow((AccX),2)+pow((AccZ),2))))*57.2958;
 angleY=(atan(AccX/sqrt(pow((AccY),2)+pow((AccZ),2))))*57.2958;

 angleX=angleX-angleErrorX;
 angleY=angleY-angleErrorY;
}

void readAccelError()
{
  for(int i=0;i<=c;i++)
  {
      // === Read acceleromter data === //
         Wire.begin();                      // Initialize comunication
  Wire.beginTransmission(MPU);       // Start communication with MPU6050 // MPU=0x68
  Wire.write(0x6B);                  // Talk to the register 6B
  Wire.write(0x00);                  // Make reset - place a 0 into the 6B register
  Wire.endTransmission(true);        //end the transmission
 
  // Configure Accelerometer Sensitivity - Full Scale Range (default +/- 2g)
  Wire.beginTransmission(MPU);
  Wire.write(0x1C);                  //Talk to the ACCEL_CONFIG register (1C hex)
  Wire.write(0x10);                  //Set the register bits as 00010000 (+/- 8g full scale range)
  Wire.endTransmission(true);
  
 //Accessing Register to activate low pass filter
        Wire.beginTransmission(MPU);
        Wire.write(0x1A);
        Wire.write(0x05); 
        Wire.endTransmission();
        
  Wire.beginTransmission(MPU);
  Wire.write(0x3B); // Start with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 6, true); // Read 6 registers total, each axis value is stored in 2 registers
  //For a range of +-2g, we need to divide the raw values by 16384, according to the datasheet
  AccX = (Wire.read() << 8 | Wire.read()) / 4096.0; // X-axis value
  AccY = (Wire.read() << 8 | Wire.read()) / 4096.0; // Y-axis value
  AccZ = (Wire.read() << 8 | Wire.read()) / 4096.0; // Z-axis value

  AccX=AccX/0.97;
  AccY=AccY/0.97;
  AccZ=AccZ/0.97;

  if(AccX>1)
  {
    AccX=1;
  }
   if(AccX<-1)
  {
    AccX=-1;
  }

  //////////////////////////////

   if(AccY>1)
  {
    AccY=1;
  }
   if(AccY<-1)
  {
    AccY=-1;
  }
  ////////////////////////////////
   if(AccZ>1)
  {
    AccZ=1;
  }
   if(AccZ<-1)
  {
    AccZ=-1;
  }
 angleX=(atan(AccY/sqrt(pow((AccX),2)+pow((AccZ),2))))*57.2958;
 angleY=(atan(AccX/sqrt(pow((AccY),2)+pow((AccZ),2))))*57.2958;
 
    angleErrorX=angleErrorX+angleX;
    angleErrorY=angleErrorY+angleY;
  }
  angleErrorX=angleErrorX/c;
  angleErrorY=angleErrorY/c; 
}
