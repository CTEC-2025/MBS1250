# MBS1250 Arduino Library

## Version: 0.0.5 ##
Minimal Arduino Library For Reading Analog Voltage From The Danfoss MBS1250 Pressure sensor (10 Bar Version).

## What's New In v0.0.5
- Added Soft Warning For Potential Invalid Analog Pin Usage On Uno/Nano

## Features
- Clean C++ Class Wrapper
- Reads Voltage From Analog Input
- Scaffold For Future Pressure Conversion Logic

## Example

#include <MBS1250.h>

MBS1250 sensor(A0);

void setup() {
	Serial.begin(9600);
}

void loop() {
	serial.println(sensor.readVoltage());
	delay(1000);
}