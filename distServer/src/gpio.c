#include <wiringPi.h>
#include <gpio.h>

void switchPin(int pin, int mode) {
  if(mode) digitalWrite(pin, HIGH);
  else digitalWrite(pin, LOW);

  return;
}

int readGPIO(int pin) {
  int value = digitalRead(pin);
  
  return value;
}