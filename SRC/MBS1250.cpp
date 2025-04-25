#include "MBS1250.h"

// EEPROM storage struct (internal use)
struct MBS1250Config {
    float vMin, vMax;
    float pMin, pMax;
    float zeroOffset;
    uint16_t checksum;
};

// Constructor
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
    _smoothingMode(SMOOTH_NONE) {}

void MBS1250::begin() {
#if defined(ARDUINO_AVR_UNO) || defined(ARDUINO_AVR_NANO)
    if (_pin < A0 || _pin > A5) {
        Serial.print("[MBS1250] Warning: Pin ");
        Serial.print(_pin);
        Serial.println(" may not be a valid analog input on Uno/Nano.");
    }
#endif
}

// -----------------------------
// Configuration
// -----------------------------
void MBS1250::enableClamping(bool on) {
    _clampEnabled = on;
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

    if (unit == "psi") return pressureBar * 14.5038;
    if (unit == "kPa") return pressureBar * 100.0;
    return pressureBar;
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
// v1.1.0: EMA Smoothing
// -----------------------------
void MBS1250::enableEMASmoothing(bool enabled, float alpha) {
    _emaEnabled = enabled;
    _emaAlpha = constrain(alpha, 0.01, 0.99);
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

    if (unit == "psi") return _emaPressure * 14.5038;
    if (unit == "kPa") return _emaPressure * 100.0;
    return _emaPressure;
}

// -----------------------------
// v1.2.0: Unified Smoothing Mode
// -----------------------------
void MBS1250::setSmoothingMode(SmoothingMode mode) {
    _smoothingMode = mode;
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
// v1.2.0: Sensor Status + Full Reading
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

    if (unit == "psi") data.pressure *= 14.5038;
    if (unit == "kPa") data.pressure *= 100.0;

    _lastPressure = data.pressure;
    _lastVoltage = data.voltage;

    return data;
}

// -----------------------------
// Diagnostics
// -----------------------------
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
    ADMUX = (_pin & 0x07) | (1 << REFS0) |
            (1 << MUX3) | (1 << MUX2) | (1 << MUX1);
    delay(2);
    ADCSRA |= (1 << ADSC);
    while (ADCSRA & (1 << ADSC));
    return (1.1 * 1023.0) / ADC;
#else
    return -1.0;
#endif
}

// -----------------------------
// Internal conversion helper
// -----------------------------
float MBS1250::_voltageToPressure(float voltage) {
    return (voltage - _vMin) * ((_pMax - _pMin) / (_vMax - _vMin)) + _pMin;
}