#include "MBS1250.h"

MBS1250::MBS1250(uint8_t pin, float vRef) {
	_pin = pin;
	_vRef = vRef;
	_clampEnabled = true;

	#if defined(ARDUINO_AVR_UNO) || defined(ARDUINO_AVR_NANO)
		if (_pin < A0 || _pin > A5) {
			Serial.print("[MBS1250] Warning: Pin ");
			Serial.print(_pin);
			Serial.println(" May Not Be An Analog Input On Uno/Nano.");
		}
	#endif
}

void MBS1250::enableClamping(bool on) {
	_clampEnabled = on;
}

float MBS1250::readVoltage() {
	float voltage = analogRead(_pin) * (_vRef / 1023.0);
	
	if (_clampEnabled) {
		if (voltage < 0.5) voltage = 0.5;
		if (voltage > 4.5) voltage = 4.5;
	}
	
	return voltage;
}

float MBS1250::readPressure() {
	float voltage = readVoltage();
	return (voltage - 0.5) * (10.0 / (4.5 - 0.5));
}