#include <MBS1250.h>

MBS1250 sensor(A0);

void setup() {
	Serial.begin(9600);
}

void loop() {
	Serial.print("Pressure: ");
	Serial.print(sensor.readPressure(), 2);
	Serial.println(" Bar");
	
	delay(1000);
}