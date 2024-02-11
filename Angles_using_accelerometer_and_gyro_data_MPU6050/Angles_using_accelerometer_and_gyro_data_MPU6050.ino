#include <Wire.h>
#include<math.h>
const int mpu = 0x68; // MPU6050 I2C address
float AccX, AccY, AccZ;
float angleX,angleY;
int c=200;
float angleErrorX=0,angleErrorY=0;
float roll,pitch,yaw;
float rollError=0,pitchError=0,yawError=0;


void setup()
{
  Serial.begin(9600);
  //Starting mpu6050 in power mode
  Wire.beginTransmission(mpu);
  Wire.write(0x6B);
  Wire.write(0x00);  //All bits in this register has to be set to zero for the device to start and continue in power mode
  Wire.endTransmission();
  delay(20);
  readMPUError();
  
}

void loop()
{
  readMPUData();
  angleX-=angleErrorX;
  angleY-=angleErrorY;

    roll-=rollError;
    pitch-=pitchError;
    yaw-=yawError;
 //Appliying Compelimentey filters on Angles
 roll=0.98*angleX + 0.02*roll;
 pitch=0.98*angleY + 0.02*pitch;
 
  Serial.print("roll ");
  Serial.print(roll);
  Serial.print(" | ");
  Serial.print("pitch ");
  Serial.println(pitch);
//  Serial.print(" | ");
//  Serial.print("yaw ");
//  Serial.println(yaw);


//  delay(100);
}

void readMPUData(void)
{
  //Activitating Low pass filter
  Wire.beginTransmission(mpu);
  Wire.write(0x1A);
  Wire.write(0x04);
  Wire.endTransmission();

  //Reading Accelerometer Data
  // Configure Accelerometer Sensitivity - Full Scale Range (default +/- 2g)
  Wire.beginTransmission(mpu);
  Wire.write(0x1C);                  //Talk to the ACCEL_CONFIG register (1C hex)
  Wire.write(0x10);                  //Set the register bits as 00010000 (+/- 8g full scale range)
  Wire.endTransmission(true);
  //Accessing registers to store accelerometer data     
  Wire.beginTransmission(mpu);
  Wire.write(0x3B);
  Wire.endTransmission();
  //Requesting 6 bytes from mpu6050 to pull accelerometer measurement information
  Wire.requestFrom(mpu, 6, true); // Read 6 registers total, each axis value is stored in 2 registers
  //For a range of +-2g, we need to divide the raw values by 16384, according to the datasheet
  AccX = (Wire.read() << 8 | Wire.read()) / 4096.0; // X-axis value
  AccY = (Wire.read() << 8 | Wire.read()) / 4096.0; // Y-axis value
  AccZ = (Wire.read() << 8 | Wire.read()) / 4096.0; // Z-axis value
  // making acceleration in x-axis or y-axis or z-axis equal to 0 expect on which axis acceleration exprienced is equal to 1
  AccX=AccX/0.97;
  AccY=AccY/0.97;
  AccZ=AccZ/0.97;
  //Limitting acceleration value from -1 to 1 in all axies
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

 //Calculating angles using Accelerometer data in all 3-axies and converting radian into angle by multiplying with 57.2958
 angleX=(atan(AccY/sqrt(pow((AccX),2)+pow((AccZ),2))))*57.2958;
 angleY=(atan(AccX/sqrt(pow((AccY),2)+pow((AccZ),2))))*57.2958;

 //Reading Gyro data
 //Setting the sensitivity scale factor for gyrometer
  Wire.beginTransmission(mpu);
  Wire.write(0x1B);
  Wire.write(0x8);
  Wire.endTransmission();
  //Accessing registers to store gyro measurement
  Wire.beginTransmission(mpu);
  Wire.write(0x43);
  Wire.endTransmission();
  //Requesting 6 bytes from mpu6050 to pull Gyro measurement information
  Wire.requestFrom(mpu,6);
  roll=Wire.read()<<8 | Wire.read();
  pitch=Wire.read()<<8 | Wire.read();
  yaw=Wire.read()<<8 | Wire.read();
  //Gyro measurement are in LSB so to converting it into degree/sec
  roll=roll/65.5;
  pitch=pitch/65.5;
  yaw=yaw/65.5;
}

void readMPUError(void)
{
  Serial.println("Dont't move MPU6050 callibration Accelerometer and Gyro data");
  for(int i=0;i<c;i++)
  {
    readMPUData();
    angleErrorX+=angleX;
    angleErrorY+=angleY;
    
    rollError+=roll;
    pitchError+=pitch;
    yawError+=yaw;
    delay(1);
  }

  angleErrorX=angleErrorX/c;
  angleErrorY=angleErrorY/c;

  rollError/=c;
  pitchError/=c;
  yawError/=c;
  Serial.println("Done callibrationg Accelerometer and Gyro data");
}
