#include <Wire.h>
int16_t Acc_rawX, Acc_rawY, Acc_rawZ,Gyr_rawX, Gyr_rawY, Gyr_rawZ;
 
float Acceleration_angleX,Acceleration_angleY;
float Gyro_angleX,Gyro_angleY;
float Total_angleX,Total_angleY;

float changeInTime, time, timePrev;
float P=0,I=0,D=0,PID=0;
float Pk=3.55;
float Ik=0.005;
float Dk=2;
float area=0,slope=0,error,changeInError,previousError,desiredAngle=0,actualAngle=0;

void setup() {
  Wire.begin(); //begin the wire comunication
  Wire.beginTransmission(0x68);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
  Serial.begin(9600);
  time = millis(); //Start counting time in milliseconds
  
  }

void loop() {
    timePrev = time;  // the previous time is stored before the actual time read
    time = millis();  // actual time read
    changeInTime = (time - timePrev) / 1000; 
    readMPUData();
    
    actualAngle=Total_angleX;
    error=desiredAngle-actualAngle;
    changeInError=error-previousError;
    slope=changeInError/changeInTime;

    
    //Calculating proportional controller value
    P=Pk*error;
    //Calculating Differential controller value
    D=Dk*slope;
    //Calculating Integral controller value
    if(-3<error<3)
    {
      I=I+(Ik*error);
    }
//Calculating PID value    
PID=P+I+D;
//limiting PID value from -1000 to 1000     
if(PID < -1000)
{
  PID=-1000;
}
if(PID > 1000)
{
  PID=1000;
}

   Serial.print(Total_angleX);
   Serial.print(" | ");
   Serial.print(Total_angleY);
   Serial.print("   ");
   Serial.println(PID);
   
   previousError=error;
}

void readMPUData(void)
  {
     //Activitating Low pass filter
     Wire.beginTransmission(0x68);
     Wire.write(0x1A);
     Wire.write(0x04);
     Wire.endTransmission();
  
     Wire.beginTransmission(0x68);
     Wire.write(0x3B); //Ask for the 0x3B register- correspond to AcX
     Wire.endTransmission(false);
     Wire.requestFrom(0x68,6,true); 
   
     Acc_rawX=Wire.read()<<8|Wire.read(); //each value needs two registres
     Acc_rawY=Wire.read()<<8|Wire.read();
     Acc_rawZ=Wire.read()<<8|Wire.read();

     Acc_rawX/16384.0;
     Acc_rawY/16384.0;
     Acc_rawZ/16384.0;

   Wire.beginTransmission(0x68);
   Wire.write(0x43); //Gyro data first adress
   Wire.endTransmission(false);
   Wire.requestFrom(0x68,4,true); //Just 4 registers
   
   Gyr_rawX=Wire.read()<<8|Wire.read(); //Once again we shif and sum
   Gyr_rawY=Wire.read()<<8|Wire.read();
 
   Gyro_angleX = Gyr_rawX/131.0; 

   Gyro_angleX= Gyr_rawY/131.0;

   Acceleration_angleX = atan((Acc_rawY)/sqrt(pow((Acc_rawX),2) + pow((Acc_rawZ),2)))*57.2958;
    
   Acceleration_angleY = atan(-1*(Acc_rawX)/sqrt(pow((Acc_rawY),2) + pow((Acc_rawZ),2)))*57.2958;

   Total_angleX = 0.98 *(Total_angleX + Gyro_angleX*changeInTime) + 0.02*Acceleration_angleX;
   
   Total_angleY = 0.98 *(Total_angleY + Gyro_angleY*changeInTime) + 0.02*Acceleration_angleY;

  }
