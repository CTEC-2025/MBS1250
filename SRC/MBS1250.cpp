#include "MBS1250.h"

MBS1250::MBS1250(uint8_t pin) {
	_pin = pin;
}

#if defined(ARDUINO_AVR_UNO || defined(ARDUINO_AVR_NANO)
	if (_pin < A0 || _pin > A5) {
		Serial.println("[MBS1250] Warning; This May Not Be An Anlog Input Pin.");
	}
#endif

float MBS1250::readVoltage() {
	return analogRead(_pin) * (5.0 /1023);
}

float MBS1250::readPressure() {
	float voltage = readVoltage();
	// 0.5V = 0 Bar, 4.5V = 10Bar
	return (voltage - 0.5) * (10.0 / (4.5 - 0.5));
}