#ifndef MBS1250_H
#define MBS1250_H

#include <Arduino.h>
#include <EEPROM.h>

/*
 * MBS1250 Pressure Sensor Library
 * --------------------------------
 * Version: 1.5.0
 * Author: CTEC 2025
 *
 * Description:
 * Flexible library for MBS1250 pressure sensors with full support for 
 * smoothing, calibration, pressure stability detection, alarms, and 
 * multiple sensor handling.
 *
 * Change History:
 * - v1.0.0: Initial stable release.
 * - v1.1.0: Diagnostics, clamping, EMA smoothing.
 * - v1.2.0: Sensor health status, smoothing modes, struct-based reading.
 * - v1.2.5: Code cleanup, documentation polish.
 * - v1.3.0: Auto-zero on startup, expanded pressure unit support.
 * - v1.4.0: Pressure rate, peak/lowest pressure tracking.
 * - v1.5.0: Pressure alarms, stability detection, multiple sensor ready.
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
    void enableAutoZero(bool on);
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
    void setSmoothingMode(SmoothingMode mode);
    float readPressureEMA(const String& unit = "bar");
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

    // v1.4.0 Features
    float getPressureRate();
    float getPeakPressure();
    float getLowestPressure();
    void resetPeakHold();

    // v1.5.0 Features: Pressure Alarms
    void setHighPressureAlarm(float pressure, const String& unit = "bar");
    void setLowPressureAlarm(float pressure, const String& unit = "bar");
    void clearPressureAlarms();
    bool isHighPressureAlarmTriggered();
    bool isLowPressureAlarmTriggered();
    void onAlarmTriggered(void (*callback)());

    // v1.5.0 Features: Pressure Stability
    bool isPressureStable(unsigned long stableTimeMs = 3000, float threshold = 0.05);

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

    bool _autoZeroEnabled;
    SmoothingMode _smoothingMode = SMOOTH_NONE;

    float _previousPressure;
    unsigned long _previousTime;
    float _peakPressure;
    float _lowestPressure;

    // v1.5.0 Private Variables: Alarms
    float _highPressureAlarm;
    float _lowPressureAlarm;
    bool _highAlarmTriggered;
    bool _lowAlarmTriggered;
    void (*_alarmCallback)();

    // v1.5.0 Private Variables: Stability
    float _stableLastPressure;
    unsigned long _stableLastChangeTime;

    // Helpers
    float _voltageToPressure(float voltage);
    float _convertPressureUnit(float pressureBar, const String& unit);
};

#endif  // MBS1250_H
