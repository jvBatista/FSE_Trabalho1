#ifndef GPIO_H_
#define GPIO_H_
#include "cJSON.h"

void switchPin(int pin, int mode);
int readGPIO(int pin);

#endif 