#ifndef MBS1250_H
#define MBS1250_H

#include <Arduino.h>

class MBS1250 {
public:
	MBS1250(uint8_t pin, float vRef =5.0);
	
	void begin();									
	void enableClamping(bool on);
	
	float readVoltage();							// Clamped Voltage (Default)
	float readRawPressure();						// Raw Unclamped Pressure In BAR
	float readPressure(const String& unit = "BAR");	// Clamped + Scaled Pressure 
	float readSmoothedPressure(int samples = 10, const String& unit = "bar");
	
	float getPressureMin();
	float getPressureMax();
	
	void setZeroOffset(float offsetBar);
	bool isPressureOutOfRange();					// Simple Diagnostic Method
	bool isSensorConnected();
	float getSupplyVoltage();						// AVR Only
	
private:
	uint8_t _pin;
	float _vRef;
	bool _clampEnabled = true;
	float _zeroOffset;
	float _pMin;
	float _pMax;
};

#endif