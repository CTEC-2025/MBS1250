#ifndef MBS1250_H
#define MBS1250_H

#include <Arduino.h>
#include <EEPROM.h>

class MBS1250 {
public:
    MBS1250(uint8_t pin, float vRef = 5.0);
    void begin();

    // Configuration
    void enableClamping(bool on);
    void setZeroOffset(float offsetBar);
    void setCalibration(float vMin, float vMax, float pMin, float pMax);
    void resetCalibration();

    // EEPROM
    void saveCalibrationToEEPROM();
    void loadCalibrationFromEEPROM();

    // Readings
    float readVoltage();                                      				  // Clamped voltage
    float readRawPressure();                                 				  // Unclamped pressure
    float readPressure(const String& unit = "bar");           				  // With zero offset
    float readSmoothedPressure(int samples = 10, const String& unit = "bar");
	
	// V1.1.0: Advanced Smoothing
	void enableEMASmoothing(bool enabled, float alpha = 0.2);
	float readPressureEMA(const String& unit = "bar");
	
	// V1.1.0: Diagnostics & Debug
	bool isClamped();														  // True If Last Voltage Was Clamped
	float getLastVoltage();													  // Last Voltage Read
	float getLastPressure();												  // Last Pressure In BAR
	void enableDebug(bool on);												  // Enables Serial Debug Output

    // Diagnostics
    bool isPressureOutOfRange();
    bool isSensorConnected();
    float getSupplyVoltage();  												  // AVR only

    // Accessors
    float getPressureMin();
    float getPressureMax();
    float getVoltageMin();
    float getVoltageMax();

private:
    uint8_t _pin;
    float _vRef;
    bool _clampEnabled;
    float _zeroOffset;
    float _vMin, _vMax;
    float _pMin, _pMax;

	// V1.1.0 State
	bool _debugEnabled;
	bool _clampedLastRead;
	bool _emaEnabled;
	float _emaAlpha;
	float _emaPressure;
	float _lastVoltage;
	float _lastPressure;

    float _voltageToPressure(float voltage);
};

#endif  // MBS1250_H