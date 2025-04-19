#include <MBS1250.h>

MBS1250 sensor(A0, 5.0);

void setup() {
	Serial.begin(9600);
	sensor.begin();
}

void loop() {
	float pressure = sensor.readPressure();
	bool outOfRange = sensor.isPressureOutOfRange();
	
	Serial.print("Pressure: ");
	Serial.print(pressure, 2);
	Serial.print(" BAR");
	
	if (outOfRange) {
		Serial.print("  [Warning: Voltage Out Of Range!]");
	}
	
	Serial.println();
	delay(1000);
}