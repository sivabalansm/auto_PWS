#ifndef TEMP_SENOR_H
#define TEMP_SENOR_H
#include <DHT.h>

void initializeDHT(char pinNum);
void setupDHT();
float getHumidity();
float getTemperature();

#endif
