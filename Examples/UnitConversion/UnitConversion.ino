#include <MBS1250.h>

MBS1250 sensor(A0, 5.0);

void setup() {
	Serial.begin(9600);
	delay(500);
}
	
void loop () {
	float pBar = sensor.readPressure("BAR");
	float pPsi = sensor.readPressure("psi");
	float pKpa = sensor.readPressure("kPa");
	
	Serial.print("Pressure: ");
	Serial.print(pBar, 2); Serial.print(" BAR | ");
	Serial.print(pPsi, 2); Serial.print(" psi | ");
	Serial.print(pKpa, 1); Serial.println(" kPa");
	
	delay(1000);
}