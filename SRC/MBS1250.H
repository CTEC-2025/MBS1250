#ifndef MBS1250_H
#define MBS1250_H

#include <Arduino.h>

class MBS1250 {
public:
	MBS1250(uint8_t pin, float vRef =5.0);
	
	void begin();									//Standard Arduino-Style Init Method
	void enableClamping(bool on);
	
	float readVoltage();							// Clamped Voltage (0.5-4.5V)
	float readRawPressure();						// Raw Unclamped Pressure In BAR
	float readPressure(const String& unit = "BAR");	
	
	float getPressureMin();
	float getPressureMax();
	
private:
	uint8_t _pin;
	float _vRef;
	bool _clampEnabled = true;
	
	float _pMin;
	float _pMax;
};

#endif