#include <MBS1250.h>

MBS1250 sensor(A0);

void setup() {
	Serial.begin(9600);
}

void loop() {
	Serial.println(sensor.readVoltage());
	delay(1000);
}