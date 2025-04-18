#include <MBS1250.h>

// Use Custom Vref If Needed (e.g. For 3.3V Boards)
MBS1250 sensor(A0);   // Or: MBS1250 sensor(A0,3.3);

void setup() {
	Serial.begin(9600);
}

void loop() {
	Serial.print("Pressure: ");
	Serial.print(sensor.readPressure(), 2);
	Serial.println(" Bar");
	
	delay(1000);
}