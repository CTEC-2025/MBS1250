#include <MBS1250.h>

MBS1250 sensor(A0, 3.3); // For Use On 3.3V Boards

void setup() {
	Serial.begin(9600);
	delay(1000);
	
	// Optional: Disable Voltage Clamping For Diagnostics
	// sensor.enableClamping(true);
}

void loop() {
	float voltage = sensor.readVoltage();
	float pressure = sensor.readPressure();
	
	Serial.print("Voltage: ");
	Serial.print(voltage, 3);
	Serial.print(" V | Pressure: ");
	Serial.print(pressure, 2);
	Serial.println(" BAR");
	
	delay(1000);
}