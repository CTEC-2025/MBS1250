#include <MBS1250.h>

MBS1250 sensor(A0, 5.0);				// A0 Input, 5V Reference

void setup() {
	Serial.begin(9600);
	sensor.begin();
										// Apply A Small Zero Offset If Known (Optional)
	sensor.setZeroOffset(-0.05);		// Calibrate Out 0.05 Bar Drift
}

void loop() {
										// Basic Sensor Connection Check
	if (!sensor.isSensorConnected()) {
		Serial.println("[ERROR] Sensor May Be Disconnected Or Wiring Fault Detected!");
	}
	
										// Get Smoothed Pressure Reading (Default 10 Samples)
	float pressure = sensor.readSmoothedPressure(10, "bar");
	
										// Check For Out Of Range Readings
	bool outOfRange = sensor.isPressureOutOfRange();
	
	Serial.print("Smoothed Pressure: ");
	Serial.print(pressure, 2);
	Serial.println(" bar");
	
	if (outOfRange) {
		Serial.println("[Warning] Sensor Voltage Out Of Valid Range (0.45V-4.55V)");
	}
	
	Serial.println();
	delay(1500);
}