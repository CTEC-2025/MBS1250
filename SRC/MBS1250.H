#ifndef MBS1250_H
#define MBS1250_H

#include <Arduino.h>
#include <EEPROM.h>

// v1.2.0: Sensor health states
enum SensorStatus {
    SENSOR_OK,
    SENSOR_DISCONNECTED,
    SENSOR_CLAMPED,
    SENSOR_OUT_OF_RANGE
};

// v1.2.0: Reading structure
struct PressureData {
    float pressure;
    float voltage;
    bool clamped;
    bool connected;
};

// v1.2.0: Smoothing strategy
enum SmoothingMode {
    SMOOTH_NONE,
    SMOOTH_AVERAGE,
    SMOOTH_EMA
};

class MBS1250 {
public:
    MBS1250(uint8_t pin, float vRef = 5.0);
    void begin();

    // Configuration
    void enableClamping(bool on);
    void setZeroOffset(float offsetBar);
    void setCalibration(float vMin, float vMax, float pMin, float pMax);
    void resetCalibration();
    void printCalibration();  // v1.2.0

    // EEPROM
    void saveCalibrationToEEPROM();
    void loadCalibrationFromEEPROM();

    // Readings
    float readVoltage();
    float readRawPressure();
    float readPressure(const String& unit = "bar");
    float readSmoothedPressure(int samples = 10, const String& unit = "bar");

    // v1.1.0: EMA smoothing
    void enableEMASmoothing(bool enabled, float alpha = 0.2);
    float readPressureEMA(const String& unit = "bar");

    // v1.2.0: Unified smoothing strategy
    void setSmoothingMode(SmoothingMode mode);
    float readPressureSmoothed(const String& unit = "bar");

    // v1.2.0: Status + struct-based reading
    SensorStatus getSensorStatus();
    PressureData getReading(const String& unit = "bar");

    // v1.1.0: Diagnostics & debug
    bool isClamped();
    float getLastVoltage();
    float getLastPressure();
    void enableDebug(bool on);

    // Diagnostics
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

    // v1.1.0
    bool _debugEnabled;
    bool _clampedLastRead;
    bool _emaEnabled;
    float _emaAlpha;
    float _emaPressure;
    float _lastVoltage;
    float _lastPressure;

    // v1.2.0
    SmoothingMode _smoothingMode = SMOOTH_NONE;

    float _voltageToPressure(float voltage);
};

#endif  // MBS1250_H