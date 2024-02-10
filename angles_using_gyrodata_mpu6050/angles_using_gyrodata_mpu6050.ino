#include<Wire.h>
#define mpu 0x68
float roll,pitch,yaw;
float rollError=0,pitchError=0,yawError=0;
int c=2000;
void gyroData(void)
{
  //Activitating Low pass filter
  Wire.beginTransmission(mpu);
  Wire.write(0x1A);
  Wire.write(0x05);
  Wire.endTransmission();
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

void gyroError(void)
{
  Serial.println("Don't move MPU6050 calculating gyroError");
  for(int i=0;i<c;i++)
  {
    gyroData();
    rollError+=roll;
    pitchError+=pitch;
    yawError+=yaw;
    delay(1);
  }
  rollError/=c;
  pitchError/=c;
  yawError/=c;
  Serial.println("Finished calculating gyroError");
}

void setup()
{
  Serial.begin(9600);
  //Starting mpu6050 in power mode
  Wire.beginTransmission(mpu);
  Wire.write(0x6B);
  Wire.write(0x00);  //All bits in this register has to be set to zero for the device to start and continue in power mode
  Wire.endTransmission();
  delay(20);
  gyroError();
}

void loop(){
  gyroData();
  roll-=rollError;
  pitch-=pitchError;
  yaw-=yawError;
  Serial.print("roll ");
  Serial.print(roll);
  Serial.print(" | ");
  Serial.print("pitch ");
  Serial.print(pitch);
  Serial.print(" | ");
  Serial.print("yaw ");
  Serial.println(yaw);
  delay(30);
}
