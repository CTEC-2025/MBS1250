#include <MBS1250.h>

MBS1250 Sensor(A0, 5.0);

void setup() {
	Serial.begin(9600);
	Sensor.begin();			// Optional, Provides Pin Check Warning
}

void loop() {
	float raw = Sensor.readRawPressure();
	float filtered = Sensor.readPressure();
	
	Serial.print("Raw Pressure: ");
	Serial.print(raw, 2);
	Serial.print(" BAR | Clamped: ");
	Serial.print(filtered, 2);
	Serial.println( "BAR");
	
	delay(1000);
}