#include "MBS1250.h"

/*
 * MBS1250 Pressure Sensor Library
 * --------------------------------
 * Version: 1.5.0
 * Author: CTEC 2025
 */

// EEPROM storage struct
struct MBS1250Config {
    float vMin, vMax;
    float pMin, pMax;
    float zeroOffset;
    uint16_t checksum;
};

// -----------------------------
// Constructor
// -----------------------------
MBS1250::MBS1250(uint8_t pin, float vRef)
  : _pin(pin), _vRef(vRef),
    _clampEnabled(true),
    _zeroOffset(0.0),
    _vMin(0.5), _vMax(4.5),
    _pMin(0.0), _pMax(10.0),
    _debugEnabled(false),
    _clampedLastRead(false),
    _emaEnabled(false),
    _emaAlpha(0.2),
    _emaPressure(0.0),
    _lastVoltage(0.0),
    _lastPressure(0.0),
    _autoZeroEnabled(false),
    _previousPressure(0.0),
    _previousTime(0),
    _peakPressure(-9999.0),
    _lowestPressure(9999.0),
    _highPressureAlarm(-1.0),
    _lowPressureAlarm(-1.0),
    _highAlarmTriggered(false),
    _lowAlarmTriggered(false),
    _alarmCallback(nullptr),
    _stableLastPressure(0.0),
    _stableLastChangeTime(0),
    _smoothingMode(SMOOTH_NONE) {}

void MBS1250::begin() {
#if defined(ARDUINO_AVR_UNO) || defined(ARDUINO_AVR_NANO)
    if (_pin < A0 || _pin > A5) {
        Serial.print("[MBS1250] Warning: Pin ");
        Serial.print(_pin);
        Serial.println(" may not be a valid analog input on Uno/Nano.");
    }
#endif

    if (_autoZeroEnabled) {
        float initialPressure = _voltageToPressure(readVoltage());
        setZeroOffset(-initialPressure);
        if (_debugEnabled) {
            Serial.print("[MBS1250] Auto-zero applied. Offset: ");
            Serial.println(_zeroOffset, 3);
        }
    }

    _previousPressure = 0.0;
    _previousTime = millis();
    _peakPressure = -9999.0;
    _lowestPressure = 9999.0;

    _highPressureAlarm = -1.0;
    _lowPressureAlarm = -1.0;
    _highAlarmTriggered = false;
    _lowAlarmTriggered = false;
    _alarmCallback = nullptr;

    _stableLastPressure = 0.0;
    _stableLastChangeTime = millis();
}

// -----------------------------
// Configuration
// -----------------------------
void MBS1250::enableClamping(bool on) {
    _clampEnabled = on;
}

void MBS1250::enableAutoZero(bool on) {
    _autoZeroEnabled = on;
}

void MBS1250::setZeroOffset(float offsetBar) {
    _zeroOffset = offsetBar;
}

void MBS1250::setCalibration(float vMin, float vMax, float pMin, float pMax) {
    _vMin = vMin;
    _vMax = vMax;
    _pMin = pMin;
    _pMax = pMax;
}

void MBS1250::resetCalibration() {
    _vMin = 0.5;
    _vMax = 4.5;
    _pMin = 0.0;
    _pMax = 10.0;
}

void MBS1250::printCalibration() {
    Serial.println(F("[MBS1250] Current Calibration:"));
    Serial.print(F("Voltage Range: "));
    Serial.print(_vMin, 3);
    Serial.print("V - ");
    Serial.print(_vMax, 3);
    Serial.println("V");

    Serial.print(F("Pressure Range: "));
    Serial.print(_pMin, 2);
    Serial.print(" - ");
    Serial.print(_pMax, 2);
    Serial.println(" bar");
}

// -----------------------------
// EEPROM
// -----------------------------
void MBS1250::saveCalibrationToEEPROM() {
    MBS1250Config config = {_vMin, _vMax, _pMin, _pMax, _zeroOffset, 0xABCD};
    EEPROM.put(0, config);
}

void MBS1250::loadCalibrationFromEEPROM() {
    MBS1250Config config;
    EEPROM.get(0, config);
    if (config.checksum == 0xABCD) {
        _vMin = config.vMin;
        _vMax = config.vMax;
        _pMin = config.pMin;
        _pMax = config.pMax;
        _zeroOffset = config.zeroOffset;
    } else {
        Serial.println("[MBS1250] EEPROM checksum invalid. Using defaults.");
    }
}

// -----------------------------
// Readings
// -----------------------------
float MBS1250::readVoltage() {
    float voltage = analogRead(_pin) * (_vRef / 1023.0);
    _lastVoltage = voltage;
    _clampedLastRead = false;

    if (_clampEnabled) {
        if (voltage < _vMin) {
            voltage = _vMin;
            _clampedLastRead = true;
        }
        if (voltage > _vMax) {
            voltage = _vMax;
            _clampedLastRead = true;
        }
    }

    if (_debugEnabled) {
        Serial.print("[MBS1250] Voltage: ");
        Serial.println(voltage, 3);
    }

    return voltage;
}

float MBS1250::readRawPressure() {
    float rawVoltage = analogRead(_pin) * (_vRef / 1023.0);
    return _voltageToPressure(rawVoltage);
}

float MBS1250::readPressure(const String& unit) {
    float pressureBar = _voltageToPressure(readVoltage()) + _zeroOffset;
    _lastPressure = pressureBar;

    unsigned long currentTime = millis();
    unsigned long deltaTime = currentTime - _previousTime;
    if (deltaTime > 0) {
        _previousTime = currentTime;
        _previousPressure = pressureBar;
    }

    if (pressureBar > _peakPressure) _peakPressure = pressureBar;
    if (pressureBar < _lowestPressure) _lowestPressure = pressureBar;

    // --- Alarms ---
    if (_highPressureAlarm > 0 && pressureBar >= _highPressureAlarm) {
        if (!_highAlarmTriggered) {
            _highAlarmTriggered = true;
            if (_alarmCallback != nullptr) _alarmCallback();
        }
    }

    if (_lowPressureAlarm > 0 && pressureBar <= _lowPressureAlarm) {
        if (!_lowAlarmTriggered) {
            _lowAlarmTriggered = true;
            if (_alarmCallback != nullptr) _alarmCallback();
        }
    }

    // --- Stability ---
    if (abs(pressureBar - _stableLastPressure) > 0.02) {
        _stableLastChangeTime = currentTime;
        _stableLastPressure = pressureBar;
    }

    return _convertPressureUnit(pressureBar, unit);
}

float MBS1250::readSmoothedPressure(int samples, const String& unit) {
    if (samples < 1) samples = 1;
    if (samples > 100) samples = 100;

    float sum = 0.0;
    for (int i = 0; i < samples; i++) {
        sum += readPressure(unit);
        delay(2);
    }
    return sum / samples;
}

// -----------------------------
// Smoothing
// -----------------------------
void MBS1250::enableEMASmoothing(bool enabled, float alpha) {
    _emaEnabled = enabled;
    _emaAlpha = constrain(alpha, 0.01, 0.99);
}

void MBS1250::setSmoothingMode(SmoothingMode mode) {
    _smoothingMode = mode;
}

float MBS1250::readPressureEMA(const String& unit) {
    float current = _voltageToPressure(readVoltage()) + _zeroOffset;
    if (_emaEnabled) {
        _emaPressure = (_emaAlpha * current) + ((1.0 - _emaAlpha) * _emaPressure);
    } else {
        _emaPressure = current;
    }

    _lastPressure = _emaPressure;

    if (_debugEnabled) {
        Serial.print("[MBS1250] EMA: ");
        Serial.println(_emaPressure, 3);
    }

    return _convertPressureUnit(_emaPressure, unit);
}

float MBS1250::readPressureSmoothed(const String& unit) {
    switch (_smoothingMode) {
        case SMOOTH_AVERAGE:
            return readSmoothedPressure(10, unit);
        case SMOOTH_EMA:
            return readPressureEMA(unit);
        case SMOOTH_NONE:
        default:
            return readPressure(unit);
    }
}

// -----------------------------
// Diagnostics and Reading Struct
// -----------------------------
SensorStatus MBS1250::getSensorStatus() {
    if (!isSensorConnected()) return SENSOR_DISCONNECTED;
    if (_clampedLastRead) return SENSOR_CLAMPED;
    if (isPressureOutOfRange()) return SENSOR_OUT_OF_RANGE;
    return SENSOR_OK;
}

PressureData MBS1250::getReading(const String& unit) {
    PressureData data;
    data.voltage = readVoltage();
    data.pressure = _voltageToPressure(data.voltage) + _zeroOffset;
    data.connected = isSensorConnected();
    data.clamped = _clampedLastRead;

    _lastPressure = data.pressure;
    _lastVoltage = data.voltage;

    data.pressure = _convertPressureUnit(data.pressure, unit);

    return data;
}

bool MBS1250::isClamped() {
    return _clampedLastRead;
}

float MBS1250::getLastVoltage() {
    return _lastVoltage;
}

float MBS1250::getLastPressure() {
    return _lastPressure;
}

void MBS1250::enableDebug(bool on) {
    _debugEnabled = on;
}

bool MBS1250::isPressureOutOfRange() {
    float voltage = analogRead(_pin) * (_vRef / 1023.0);
    return (voltage < (_vMin - 0.05) || voltage > (_vMax + 0.05));
}

bool MBS1250::isSensorConnected() {
    float voltage = analogRead(_pin) * (_vRef / 1023.0);
    return (voltage > 0.1 && voltage < (_vRef - 0.1));
}

float MBS1250::getSupplyVoltage() {
#if defined(__AVR__)
    ADMUX = (_pin & 0x07) | (1 << REFS0) | (1 << MUX3) | (1 << MUX2) | (1 << MUX1);
    delay(2);
    ADCSRA |= (1 << ADSC);
    while (ADCSRA & (1 << ADSC));
    return (1.1 * 1023.0) / ADC;
#else
    return -1.0;
#endif
}

// -----------------------------
// v1.4.0 Features
// -----------------------------
float MBS1250::getPressureRate() {
    unsigned long now = millis();
    float timeDiffSec = (now - _previousTime) / 1000.0;
    if (timeDiffSec == 0) return 0.0;
    return (_lastPressure - _previousPressure) / timeDiffSec;
}

float MBS1250::getPeakPressure() {
    return _peakPressure;
}

float MBS1250::getLowestPressure() {
    return _lowestPressure;
}

void MBS1250::resetPeakHold() {
    _peakPressure = -9999.0;
    _lowestPressure = 9999.0;
}

// -----------------------------
// v1.5.0 Features
// -----------------------------
void MBS1250::setHighPressureAlarm(float pressure, const String& unit) {
    _highPressureAlarm = pressure;
    if (unit != "bar") _highPressureAlarm = pressure / _convertPressureUnit(1.0, unit);
    _highAlarmTriggered = false;
}

void MBS1250::setLowPressureAlarm(float pressure, const String& unit) {
    _lowPressureAlarm = pressure;
    if (unit != "bar") _lowPressureAlarm = pressure / _convertPressureUnit(1.0, unit);
    _lowAlarmTriggered = false;
}

void MBS1250::clearPressureAlarms() {
    _highPressureAlarm = -1.0;
    _lowPressureAlarm = -1.0;
    _highAlarmTriggered = false;
    _lowAlarmTriggered = false;
}

bool MBS1250::isHighPressureAlarmTriggered() {
    return _highAlarmTriggered;
}

bool MBS1250::isLowPressureAlarmTriggered() {
    return _lowAlarmTriggered;
}

void MBS1250::onAlarmTriggered(void (*callback)()) {
    _alarmCallback = callback;
}

bool MBS1250::isPressureStable(unsigned long stableTimeMs, float threshold) {
    unsigned long now = millis();
    float delta = abs(_lastPressure - _stableLastPressure);
    if (delta <= threshold && (now - _stableLastChangeTime) >= stableTimeMs) {
        return true;
    }
    return false;
}

// -----------------------------
// Private Helpers
// -----------------------------
float MBS1250::_voltageToPressure(float voltage) {
    return (voltage - _vMin) * ((_pMax - _pMin) / (_vMax - _vMin)) + _pMin;
}

float MBS1250::_convertPressureUnit(float pressureBar, const String& unit) {
    if (unit == "psi") return pressureBar * 14.5038;
    if (unit == "kPa") return pressureBar * 100.0;
    if (unit == "atm") return pressureBar * 0.986923;
    if (unit == "mmHg") return pressureBar * 750.062;
    return pressureBar; // Default is bar
}