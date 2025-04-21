#include "MBS1250.h"

// Struct used for EEPROM calibration storage
struct MBS1250Config {
    float vMin, vMax;
    float pMin, pMax;
    float zeroOffset;
    uint16_t checksum;
};

// Constructor
MBS1250::MBS1250(uint8_t pin, float vRef)
  : _pin(pin), _vRef(vRef), _clampEnabled(true), _zeroOffset(0.0),
    _vMin(0.5), _vMax(4.5), _pMin(0.0), _pMax(10.0) {}

// Optional pin validation for Uno/Nano
void MBS1250::begin() {
#if defined(ARDUINO_AVR_UNO) || defined(ARDUINO_AVR_NANO)
    if (_pin < A0 || _pin > A5) {
        Serial.print("[MBS1250] Warning: Pin ");
        Serial.print(_pin);
        Serial.println(" may not be a valid analog input on Uno/Nano.");
    }
#endif
}

// Configuration
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

// EEPROM: Save calibration and zeroOffset
void MBS1250::saveCalibrationToEEPROM() {
    MBS1250Config config = {
        _vMin, _vMax, _pMin, _pMax, _zeroOffset,
        0xABCD  // Simple checksum
    };
    EEPROM.put(0, config);
}

// EEPROM: Load calibration and zeroOffset
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

// Internal conversion from voltage to pressure (no clamping or offset)
float MBS1250::_voltageToPressure(float voltage) {
    return (voltage - _vMin) * ((_pMax - _pMin) / (_vMax - _vMin)) + _pMin;
}

// Read clamped voltage (0.5–4.5V default)
float MBS1250::readVoltage() {
    float voltage = analogRead(_pin) * (_vRef / 1023.0);
    if (_clampEnabled) {
        if (voltage < _vMin) voltage = _vMin;
        if (voltage > _vMax) voltage = _vMax;
    }
    return voltage;
}

// Read raw pressure from sensor (no clamping or offset)
float MBS1250::readRawPressure() {
    float rawVoltage = analogRead(_pin) * (_vRef / 1023.0);
    return _voltageToPressure(rawVoltage);
}

// Read pressure in specified unit with clamping + zeroOffset
float MBS1250::readPressure(const String& unit) {
    float pressureBar = _voltageToPressure(readVoltage()) + _zeroOffset;

    if (unit == "psi")      return pressureBar * 14.5038;
    if (unit == "kPa")      return pressureBar * 100.0;
    return pressureBar;
}

// Smoothed pressure using N samples
float MBS1250::readSmoothedPressure(int samples, const String& unit) {
    if (samples < 1) samples = 1;
    if (samples > 100) samples = 100;

    float total = 0.0;
    for (int i = 0; i < samples; i++) {
        total += readPressure(unit);
        delay(2);  // ADC settle time
    }
    return total / samples;
}

// Calibration access
float MBS1250::getPressureMin() { return _pMin; }
float MBS1250::getPressureMax() { return _pMax; }
float MBS1250::getVoltageMin()  { return _vMin; }
float MBS1250::getVoltageMax()  { return _vMax; }

// Diagnostic: Out-of-range voltage detection
bool MBS1250::isPressureOutOfRange() {
    float voltage = analogRead(_pin) * (_vRef / 1023.0);
    return (voltage < (_vMin - 0.05) || voltage > (_vMax + 0.05));
}

// Diagnostic: Sensor connection test
bool MBS1250::isSensorConnected() {
    float voltage = analogRead(_pin) * (_vRef / 1023.0);
    return (voltage > 0.1 && voltage < (_vRef - 0.1));
}

// AVR-only: Internal supply voltage read using 1.1V reference
float MBS1250::getSupplyVoltage() {
#if defined(__AVR__)
    ADMUX = (_pin & 0x07) | (1 << REFS0) |
            (1 << MUX3) | (1 << MUX2) | (1 << MUX1);  // Select 1.1V reference
    delay(2);
    ADCSRA |= (1 << ADSC);
    while (ADCSRA & (1 << ADSC));
    return (1.1 * 1023.0) / ADC;
#else
    return -1.0;
#endif
}