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

    float _voltageToPressure(float voltage);
};

#endif  // MBS1250_H