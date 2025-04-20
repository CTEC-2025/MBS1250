#include <MBS1250.h>

MBS1250 sensor(A0, 5.0);							// Analog Pin And Reference Voltage

void setup() {
	Serial.begin(9600);
	sensor.begin();									// Optional Board-Specific Analog Pin Check
	
	sensor.setZeroOffset(-0.05);
}

void loop() {
	 float voltage = sensor.readVoltage();
	float rawPressure = sensor.readRawPressure();
	float pressureBar = sensor.readPressure("bar");
	float pressurePsi = sensor.readPressure("psi");
	float pressureKPa = sensor.readPressure("kPa");
	float smoothedPressure = sensor.readSmoothedPressure(10, "bar");
	bool outOfRange = sensor.isPressureOutOfRange();
	bool connected = sensor.isSensorConnected();

	Serial.println("----- Full Feature Demo -----");
	Serial.print("Voltage (clamped): "); Serial.print(voltage, 3); Serial.println(" V");
	Serial.print("Raw Pressure: "); Serial.print(rawPressure, 2); Serial.println(" bar");
	Serial.print("Pressure (bar): "); Serial.print(pressureBar, 2); Serial.println(" bar");
	Serial.print("Pressure (psi): "); Serial.print(pressurePsi, 2); Serial.println(" psi");
	Serial.print("Pressure (kPa): "); Serial.print(pressureKPa, 1); Serial.println(" kPa");
	Serial.print("Smoothed Pressure: "); Serial.print(smoothedPressure, 2); Serial.println(" bar");
	Serial.print("Sensor Connected: "); Serial.println(connected ? "Yes" : "No");
	if (outOfRange) {
		Serial.println("[Warning] Sensor voltage out of calibrated range!");
	}
	Serial.println("------------------------------\n");

	delay(1500);
}