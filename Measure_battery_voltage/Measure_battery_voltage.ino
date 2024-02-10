
float voltage;
const float dividerValue=5; //reciprocal of voltage divider
const float maximumVoltageCapicityOfArduinoOut=5;
float voltageDividerConstant=1023/(dividerValue*maximumVoltageCapicityOfArduinoOut);
void setup() {
 Serial.begin(9600);
 pinMode(A0,INPUT); 
}

void loop() {
 voltage=(analogRead(A0))/voltageDividerConstant;
 Serial.print("Voltage: "); 
 Serial.println(voltage);
 delay(300);
}

//Arduino has 10 bit ADC(Analog to Digital Convertor)
//5 volt=2 to the power 10 -1 =1023
//0 -> 0 volt
//1023 -> 5 volt
//
//since we are using a 1/5 voltage divider
//
//voltage_divider constant =1023/5(volt)*5(voltage divider) = 40.92
