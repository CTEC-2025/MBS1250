#ifndef MBS1250_H
#define MBS1250_H

#include <Arduino.h>

class MBS1250 {
public:
	MBS1250(uint8_t, float vRef =5.0);
	float readVoltage();
	float readPressure();
	
private:
	uint8_t _pin;
	float _vRef;
};

#endif