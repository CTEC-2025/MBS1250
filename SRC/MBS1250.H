#ifndef MBS1250_H
#define MBS1250_H

#include <Arduino.h>
#include <EEPROM.h>

/*
 * MBS1250 Pressure Sensor Library
 * --------------------------------
 * Version: 1.2.5
 * Author: [CTEC-2025]
 *
 * Description:
 * A flexible library for reading, smoothing, and diagnosing 
 * pressure sensors like the MBS1250. Supports voltage scaling, 
 * clamping, zero offset adjustment, EEPROM calibration storage, 
 * and smoothing modes (Average and EMA).
 *
 * Change History:
 * - v1.0.0: Initial stable release.
 * - v1.1.0: Added diagnostics, clamping, and EMA smoothing.
 * - v1.2.0: Added sensor health status, smoothing modes, 
 *           struct-based readings, and calibration printing.
 * - v1.2.5: Code cleanup, organization, and formatting polish.
 */

// Sensor health states
enum SensorStatus {
    SENSOR_OK,
    SENSOR_DISCONNECTED,
    SENSOR_CLAMPED,
    SENSOR_OUT_OF_RANGE
};

// Reading structure
struct PressureData {
    float pressure;
    float voltage;
    bool clamped;
    bool connected;
};

// Smoothing strategies
enum SmoothingMode {
    SMOOTH_NONE,
    SMOOTH_AVERAGE,
    SMOOTH_EMA
};

class MBS1250 {
public:
    // Constructor
    MBS1250(uint8_t pin, float vRef = 5.0);
    void begin();

    // Configuration
    void enableClamping(bool on);
    void setZeroOffset(float offsetBar);
    void setCalibration(float vMin, float vMax, float pMin, float pMax);
    void resetCalibration();
    void printCalibration();    

    // EEPROM
    void saveCalibrationToEEPROM();
    void loadCalibrationFromEEPROM();

    // Readings
    float readVoltage();
    float readRawPressure();
    float readPressure(const String& unit = "bar");
    float readSmoothedPressure(int samples = 10, const String& unit = "bar");

    // Smoothing
    void enableEMASmoothing(bool enabled, float alpha = 0.2);
    float readPressureEMA(const String& unit = "bar");
    void setSmoothingMode(SmoothingMode mode);
    float readPressureSmoothed(const String& unit = "bar");

    // Diagnostics
    SensorStatus getSensorStatus();
    PressureData getReading(const String& unit = "bar");
    bool isClamped();
    float getLastVoltage();
    float getLastPressure();
    void enableDebug(bool on);
    bool isPressureOutOfRange();
    bool isSensorConnected();
    float getSupplyVoltage();

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

    bool _debugEnabled;
    bool _clampedLastRead;
    bool _emaEnabled;
    float _emaAlpha;
    float _emaPressure;
    float _lastVoltage;
    float _lastPressure;

    SmoothingMode _smoothingMode = SMOOTH_NONE;

    float _voltageToPressure(float voltage);
};

#endif  // MBS1250_H