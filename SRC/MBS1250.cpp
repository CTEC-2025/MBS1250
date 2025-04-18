#include "MBS1250.h"

MBS1250::MBS1250(uint8_t pin) {
	_pin = pin;
}

float MBS1250::readVoltage() {
	return analogRead(_pin) * (5.0 /1023);
}