#ifndef MBS1250_H
#define MBS1250_H

#include <Arduino.h>

class MBS1250 {
public:
	MBS1250(uint9_t pin);
	float readVoltage();
	float readPressure(); // New: Pressure In Bar
	
private:
	uint8_t _pin;
};

#endif