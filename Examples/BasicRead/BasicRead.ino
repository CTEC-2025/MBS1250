#include <MBS1250.h>

MBS1250 sensor(A0); // Using Default 5V vRef

void setup() {
	Serial.begin(9600);
}

void loop() {
	float pressure = sensor.readPressure();
	Serial.print("Pressure: ");
	Serial.print(pressure, 2);
	Serial.println(" Bar");	
	delay(1000);
}