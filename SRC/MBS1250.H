#ifndef MBS1250_H
#define MBS1250_H

#include <Arduino.h>

class MBS1250 {
public:
	MBS1250(uint8_t pin, float vRef =5.0);
	
	void enableClamping(bool on);
	
	float readVoltage();
	float readPressure(const String& unit = "BAR");
	
	float getPressureMin();
	float getPressureMax();
	
private:
	uint8_t _pin;
	float _vRef;
	bool _clampEnabled = true;
	
	// Sensor Pressure Range (Defaults For 10 BAR Sensor)
	const float _pMin = 0.0;
	const float _pMax = 10.0;
};

#endif