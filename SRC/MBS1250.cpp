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