# MBS1250 Arduino Library

## Version: 0.1.5 ##
A Lightweight Arduino Library For Reading Analog Voltage And Calculating Pressure From The Danfoss MBS1250 Sensor (10Bar Varient).

## What's New In v0.1.5
- Added Optional 'vRef' Voltage Reference) Parameter To Constructor For Compatibility With 3.3V Or 5V Boards
- Improved Analog Pin Warning System (More Flexible, Board-Aware)
- Fixed Code Syntax in Readme 
- General Cleanup And Small Refinements

## Features
- 'readPressure()' Converts Analog To Pressure (Linear 0-10Bar)
- 'readVoltage()' Returns Raw Analog Voltage 
- Optional Analog Reference Volatge (vRef)
- Soft Warning For Possible Invalid Pin 

## Example

#include <MBS1250.h>

MBS1250 sensor(A0, 5.0);  // A0, 5V Reference

void setup() {
	Serial.begin(9600);
}

void loop() {
	float pressure = sensor.readPressure();
	Serial.print("Pressure: ");
	Serial.print(pressure, 2);
	Serial.println(" Bar");
	
	delay(1000);
}