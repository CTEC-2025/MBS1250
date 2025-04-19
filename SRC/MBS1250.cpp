#include "MBS1250.h"

MBS1250::MBS1250(uint8_t pin, float vRef) {
	_pin = pin;
	_vRef = vRef;
	_clampEnabled = true;
	_zeroOffset = 0.0;
	
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

void MBS1250::enableClamping(bool on) {
	_clampEnabled = on;
}

void MBS1250::setZeroOffset(float offsetBar) {
	_zeroOffset = offsetBar;
}

float MBS1250::readVoltage() {
	float voltage = analogRead(_pin) * (_vRef / 1023.0);
	
	if (_clampEnabled) {
		if (voltage < 0.5) voltage = 0.5;
		if (voltage > 4.5) voltage = 4.5;
	}
	
	return voltage;
}

float MBS1250::readRawPressure() {
	float rawVoltage = analogRead(_pin) * (_vRef /1023.0);
	return (rawVoltage - 0.5) * (_pMax / (4.5 - 0.5));
}

float MBS1250::readPressure(const String& unit) {
	float voltage = readVoltage();
	float pressureBar = (voltage - 0.5) * (_pMax / (4.5 - 0.5));
	
	if (unit == "psi") {
		return pressureBar * 14.5038;
	} else if (unit == "kPa") {
		return pressureBar * 100.0;
	} else {
		return pressureBar; 
	}
}

float MBS1250::readSmoothedPressure(int samples, const String& unit) {
	float total = 0.0;
	for (int i = 0; i < samples; i++) {
		total += readPressure(unit);
		delay(2);
	}
	return total / samples;
}

float MBS1250::getPressureMin() {
	return _pMin;
}

float MBS1250::getPressureMax() {
	return _pMax;
}

bool MBS1250::isPressureOutOfRange() {
	float voltage = analogRead(_pin) * (_vRef / 1023);
	return (voltage < 0.45 || voltage > 4.55);
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