#include "MBS1250.h"

struct MBS1250Config {
	float vMin, vMax, pMin, pMax, zeroOffset;
	uint16_t checksum;
};

MBS1250::MBS1250(uint8_t pin, float vRef) {
	_pin = pin;
	_vRef = vRef;
	_clampEnabled = true;
	_zeroOffset = 0.0;
	
	// Default Calibration
	_vMin = 0.5;
	_vMax = 4.5;
	_pMin = 0.0;
	_pMax = 10.0;
}

void MBS1250::begin() {
	#if defined(ARDUINO_AVR_UNO) || defined(ARDUINO_AVR_NANO)
		if (_pin < A0 || _pin > A5) {
			Serial.print("[MBS1250] Warning: Pin ");
			Serial.print(_pin);
			Serial.println(" May Not Be An Analog Input On Uno/Nano.");
		}
	#endif
}

void MBS1250::enableClamping(bool on) { _clampEnabled = on; }
void MBS1250::setZeroOffset(float offsetBar) { _zeroOffset = offsetBar; }

void MBS1250::setCalibration(float vMin, float vMax, float pMin, float pMax)
{
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
		Serial.println("[MBS1250] EEPROM Checksum Invalid. Using Defaults.");
	}
}

float MBS1250::_voltageToPressure(float voltage) {
	return (voltage - _vMin) * ((_pMax - _pMin) / (_vMax - _vMin)) + _pMin;
}

float MBS1250::readVoltage() {
    float voltage = analogRead(_pin) * (_vRef / 1023.0);
    if (_clampEnabled) {
        if (voltage < _vMin) voltage = _vMin;
        if (voltage > _vMax) voltage = _vMax;
    }
    return voltage;
}

float MBS1250::readRawPressure() {
    float rawVoltage = analogRead(_pin) * (_vRef / 1023.0);
    return _voltageToPressure(rawVoltage);
}

float MBS1250::readPressure(const String& unit) {
    float pressureBar = _voltageToPressure(readVoltage()) + _zeroOffset;
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

float MBS1250::getPressureMin() { return _pMin; }
float MBS1250::getPressureMax() { return _pMax; }
float MBS1250::getVoltageMin() { return _vMin; }
float MBS1250::getVoltageMax() { return _vMax; }

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
		// Set ADC To Mesure Internal 1.1V Reference
		ADMUX = (_pin & 0x07) | (1 << REFS0) | (1 << MUX3) | (1 << MUX2) | (1 << MUX1);
		delay(2); // Allow Vref To Settle
		
		ADCSRA |= (1 << ADSC); // Start Conversion
		while (ADCSRA & (1 << ADSC)); // Wait For Completion
		
		uint16_t result = ADC;
		float vcc = (1.1 * 1023) / result; // Vcc In Volts
		return vcc;
	#else
		return -1.0; // Not Supported On Non-AVR
	#endif
}