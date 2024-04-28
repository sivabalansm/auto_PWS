#include "temp_sensor.h"

void initializeDHT(char pinNum) {
	DHT dht( pinNum, DHT11 );
};

void setupDHT() {
	dht.begin();
};


float getHumidity() {
	return dht.readHumidity();

};

float getTemperature() {
	return dht.readTemperature();

};
