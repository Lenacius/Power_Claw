/*
 * Projeto de Plataforma Alternativa - Power Claw
 */
#include <Wire.h>

// Finger bend values
float analogPin[5] = {0, 0, 0, 0, 0};

// Serial Output
String serialOutput;

const int  MPU1 = 0x68, // AD0 Pin disconnected/grounded
           MPU2 = 0x69; // AD0 Pin connected to 3v3

float aX, aY, aZ; // Accelerometer values
float temp;       // Temperature value    
float gX, gY, gZ; // Gyroscope values

void setup() {
  Serial.begin(9600);
  
  InitMPU(MPU1);
  InitMPU(MPU2);
}

void loop() {
  ReadAnalogPins();
  
  GetMPU(MPU1);
  GetMPU(MPU2);
}

void ReadAnalogPins(){
  for(int x = 0; x < 5; x++){
    convert_analog_input(&analogPin[x], x);
    if(x == 0)
      serialOutput = "FNG|" + (String)analogPin[0];
//      serialOutput = "FNG|0";
    else
      serialOutput += '|' + (String)analogPin[x];
//      serialOutput += "|0";
  }
  Serial.println(serialOutput);
}

void convert_analog_input(float *output, int pin){
  *output = (analogRead(pin) / 1023.0f) * 10.0f;
}

void InitMPU(const int MPU){
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B); 
  Wire.write(0); 
  Wire.endTransmission(true);
  
  delay(20);
}

void GetMPU(const int MPU){
  Wire.beginTransmission(MPU); 
  Wire.write(0x3B);
  Wire.endTransmission();
  Wire.requestFrom(MPU,14,true);
  
  while(Wire.available() < 6);
  
  aX   = (Wire.read()<<8|Wire.read()) / 16384.0;  //0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)     
  aY   = (Wire.read()<<8|Wire.read()) / 16384.0;  //0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  aZ   = (Wire.read()<<8|Wire.read()) / 16384.0;  //0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  temp = (Wire.read()<<8|Wire.read());  //0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  gX   = (Wire.read()<<8|Wire.read()) / 131.0;  //0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  gY   = (Wire.read()<<8|Wire.read()) / 131.0;  //0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  gZ   = (Wire.read()<<8|Wire.read()) / 131.0;  //0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
  
  if(MPU == MPU1)
    serialOutput = "MPU1";
  else if (MPU == MPU2)
    serialOutput = "MPU2";

  // Envia os valores do acelerometro para a serial (X, Y, Z)
  serialOutput += '|' + (String)(aX);
  serialOutput += '|' + (String)(aY);
  serialOutput += '|' + (String)(aZ);
  
  //Envia valor da temperatura para a serial
  //Calcula a temperatura em graus Celsius
  //Serial.print(" | Tmp = "); Serial.print(Tmp/340.00+36.53 * 10));

  // Envia os valores do giroscopio para a serial (X, Y, Z)
  serialOutput += '|' + (String)(gX);
  serialOutput += '|' + (String)(-gZ);
  serialOutput += '|' + (String)(gY);

  Serial.println(serialOutput);

  while(Serial.available()) // garante a limpeza do buffer
  Serial.read();
}
