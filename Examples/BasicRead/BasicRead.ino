#include <MBS1250.h>

MBS1250 sensor(A0, 5.0);   // You Can Change vRef If Needed 

void setup() {
	Serial.begin(9600);
	// Optional: Disable Clamping
	// sensor.enableClamping(false);
}

void loop() {
	Serial.print("Pressure: ");
	Serial.print(sensor.readPressure(), 2);
	Serial.println(" Bar");	
	delay(1000);
}