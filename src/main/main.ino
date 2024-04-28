#include "main.h"

initializeDHT(2);

void setup() {
	setupDHT();
	Serial.begin(9600);
}

void loop() {
	Serial.println(getHumidity());
	Serial.println(getTemperature());
	delay(1000);

}
