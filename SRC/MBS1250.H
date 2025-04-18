#ifndef MBS1250_H
#define MBS1250_H

#include <Arduino.h>

class MBS1250 {
public:
	// Create Sensor Object On Specified Analog Pin, With Optional Reference Voltage
	MBS1250(uint8_t pin, float vRef =5.0);
	
	// Enable Or Disable Voltage Clamping (Default: True)
	void enableClamping(bool on);
	
	// Returns Raw Or Clamped Analog Voltage in Volts
	float readVoltage();
	
	// Converts Voltage To Pressure In Bar
	float readPressure();
	
private:
	uint8_t _pin;
	float _vRef;
	bool _clampEnabled = true;
};

#endif