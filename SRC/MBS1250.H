#ifndef MBS1250_H
#define MBS1250_H

#include <Arduino.h>

class MBS1250 {
public:
	MBS1250(uint9_t pin);
	float readVoltage();
	
private:
	uint8_t _pin;
};

#endif