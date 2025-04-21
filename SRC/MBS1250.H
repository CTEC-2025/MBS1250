#ifndef MBS1250_H
#define MBS1250_H

#include <Arduino.h>
#include <EEPROM.h>

class MBS1250 {
public:
	// Constructor: Specify Analog Input Pin And Optional Analog Reference Voltage.
	MBS1250(uint8_t pin, float vRef =5.0);
	
	// Optional: Check Anlog Pin On AVR Boards.
	void begin();								

	// Configuration:
	void enableClamping(bool on);
	void setZeroOffset(float offsetBar);
	void setCalibration(float vMin, float vMax, float pMin, float pMax);
	void resetCalibration();
	
	// EEPROM
	void saveCalibrationToEEPROM();
	void loadCalibrationFromEEPROM();

	// Pressure Readings:
	float readVoltage();							// Clamped Voltage (Default)
	float readRawPressure();						// Raw Unclamped Pressure In BAR
	float readPressure(const String& unit = "BAR");	// Clamped + Scaled Pressure 
	float readSmoothedPressure(int samples = 10, const String& unit = "bar");
	
	// Diagnostics:
	bool isPressureOutOfRange();					// Simple Diagnostic Method
	bool isSensorConnected();
	float getSupplyVoltage();						// AVR Only	
	
	// Pressure Sensor Spec Access:
	float getPressureMin();
	float getPressureMax();
	float getVoltageMin();
	float getVoltageMax();
		
private:
	uint8_t _pin;
	float _vRef;
	bool _clampEnabled = true;
	float _zeroOffset;
	
	// Calibration Parameters
	float _vMin;
	float _vMax;
	float _pMin;
	float _pMax;
	
	float _voltageToPressure(float voltage);
};

#endif