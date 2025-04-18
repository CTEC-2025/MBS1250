#include <MBS1250.h>

// Simulate Use On A 3.3V Board
MBS1250 sensor(A0, 3.3);

void setup() {
	Serial.begin(9600);
	delay(1000);
	
	// Toggle Clamping ON (Default) Or OFF For Diagmostics
	sensor.enableClamping(true);
}

void loop() {
	float voltage = sensor.readVoltage();
	float pressure = sensor.readPressure();
	
	Serial.print("Voltage: ");
	Serial.print(voltage, 3);
	Serial.print(" V    |    Pressure: ");
	Serial.print(pressure, 2);
	Serial.println(" BAR");
	
	delay(1000);
}