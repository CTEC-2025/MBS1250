#include <MBS1250.h>

MBS1250 sensor(A0, 5.0);			// Analog Pin And Reference Voltage

void setup() {
	Serial.begin(9600);
	sensor.begin();					// Optional Board-Specific Analog Pin Check
}

void loop() {
	float voltage = sensor.readVoltage();
	float rawBar = sensor.readRawPressure();
	float pressureBar = sensor.readPressure("bar");
	float pressurePsi = sensor.readPressure("psi");
	float pressureKpa = sensor.readPressure("kPa");
	bool outOfRange = sensor.isPressureOutOfRange();
	
	Serial.println("----- MBS1250 Diagnostic Readout -----");
	
	Serial.print("Voltage (Clamped): ");
	Serial.print(voltage, 3);
	Serial.println(" V");
	
	Serial.print("Pressure (Raw): ");
	Serial.print(rawBar, 2);
	Serial.println(" Bar");
	
	Serial.print("Pressure (Bar): ");
	Serial.print(pressureBar, 2);
	Serial.println(" Bar");
	
	Serial.print("Pressure (psi): ");
	Serial.print(pressurePsi, 2);
	Serial.println(" psi");
	
	Serial.print("Pressure (kPa): ");
	Serial.print(pressureKpa, 1);
	Serial.println(" kPa");
	
	Serial.print("Min Pressure: ");
	Serial.print(sensor.getPressureMin());
	Serial.print(" Bar | Max Pressure: ");
	Serial.print(sensor.getPressureMax());
	Serial.println(" Bar");
	
	Serial.print("Measured Vcc: ");
	Serial.print(sensor.getSupplyVoltage(), 3);
	Serial.println(" V");
	
	if (outOfRange) {
		Serial.println("[Warning] Sensor Voltage Out Of Range (0.45V-4.55V)");
	}
	
	Serial.println("--------------------------------------\n");
	delay(1500);
}