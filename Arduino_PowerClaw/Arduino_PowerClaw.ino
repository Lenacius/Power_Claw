/*
 * Projeto de Plataforma Alternativa - Power Claw
 */

// MPU6050 Sensor libries from Adafruit
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;

// Finger bend values
float analogPin[5] = {0, 0, 0, 0, 0};

// Serial Output
String serialOutput;

void setup() {
  SetupMPU6050();
}

void loop() {  
  ReadAnalogPins();
  MPU6050Read();
}

void ReadAnalogPins(){
  for(int x = 0; x < 5; x++){
    convert_analog_input(&analogPin[x], x);
    if(x == 0)
      serialOutput = "FNG|" + (String)analogPin[0];
    else
      serialOutput += '|' + (String)analogPin[x];
  }
  Serial.println(serialOutput);  
}

void convert_analog_input(float *output, int pin){
  *output = (analogRead(pin) / 1023.0) * 100.0;
}

void SetupMPU6050(){
  Serial.begin(115200);
  while (!Serial)
    delay(10); // will pause Zero, Leonardo, etc until serial console opens

  Serial.println("Adafruit MPU6050 test!");

  // Try to initialize!
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  // set accelerometer range to +-8G
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);

  // set gyro range to +- 500 deg/s
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);

  // set filter bandwidth to 21 Hz
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  delay(100);
}

void MPU6050Read(){  
  /* Get new sensor events with the readings */
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  serialOutput = "MPU";

  // Accelerometer uses m/s^2
  serialOutput += '|' + (String)a.acceleration.x;
  serialOutput += '|' + (String)a.acceleration.y;
  serialOutput += '|' + (String)a.acceleration.z;

  // Gyroscope uses rad/s
  serialOutput += '|' + (String)g.gyro.x;
  serialOutput += '|' + (String)g.gyro.y;
  serialOutput += '|' + (String)g.gyro.z;

  // Temperature uses degC
  serialOutput += '|' + (String)temp.temperature;

  Serial.println(serialOutput);
}
