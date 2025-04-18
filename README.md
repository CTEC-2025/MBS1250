# MBS1250 Arduino Library

## Version: 0.1.0 ##
A Lightweight Arduino Library For Reading Analog Voltage And Calculating Pressure From The Danfoss MBS1250 Sensor (10Bar Varient).

## What's New In v0.1.0
- Added 'readPressure()' Function

## Features
- Clean, Object-oriented Interface
- Reads Analog Voltage From The Sensor
- Converts the Voltage to Pressure In ##Bar##
- Configured For 5V Systems By Default
- Warns Of Invalid Analog Pin Use 

## Example

#include <MBS1250.h>

MBS1250 sensor(A0);

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