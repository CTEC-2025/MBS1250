# MBS1250 Arduino Library

## Version: 0.2.0 ##
A Lightweight Arduino Library For Reading Analog Voltage And Calculating Pressure From The Danfoss MBS1250 Sensor (10Bar Varient).

## What's New In v0.2.0
- Voltage Clamping Added To 'readVoltage()' (0.5V-4.5V Range
- Optional 'enableClamping(false)' For Diagnostic Reading
- 3.3V And 5V Reference Voltage Support
- Added 'AdvancedRead' Example

## Features
- 'readPressure()' Converts Analog To Pressure (Linear 0-10Bar)
- 'readVoltage()' Returns Raw Analog Voltage 
- Optional Analog Reference Voltagege (vRef)
- Soft Warning For Possible Invalid Pin 

## Example

#include <MBS1250.h>

MBS1250 sensor(A0, 5.0);  // A0, 5V Reference

void setup() {
	Serial.begin(9600);
	sensor.enableClamping(false); // Optional
}

void loop() {
	float pressure = sensor.readPressure();
	Serial.print("Pressure: ");
	Serial.print(pressure, 2);
	Serial.println(" Bar");
	
	delay(1000);
}