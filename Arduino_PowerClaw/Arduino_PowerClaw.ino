/*
 * Projeto de Plataforma Alternativa - Power Claw
 */

void setup() {
// Analog pin doesn't need to be initialized;
  Serial.begin(9600);
}

void loop() {
  float analogPin[5] = {0, 0, 0, 0, 0};

  for(int x = 0; x < 5; x++){
    convert_analog_input(&analogPin[x], x);
  }
}

void convert_analog_input(float *output, int pin){
  *output = (analogRead(pin) / 1023.0) * 100.0;
}
