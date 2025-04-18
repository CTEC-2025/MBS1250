# MBS1250 Arduino Library

## Version: 0.3.0 ##
A Lightweight Arduino Library For Reading Analog Voltage And Calculating Pressure From The Danfoss MBS1250 Sensor (10Bar Varient).

## What's New In v0.3.0
- Added 'readPressure(unit)' - Supports '"BAR"' (Default), '"psi"', '"kPa"'
- New Methods: 'getPressureMin()' And 'getPressureMax()'
- Added New Example: 'UnitConversion.ino'

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