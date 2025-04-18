# MBS1250 Arduino Library

## Version: 0.6.0 ##
A Lightweight Arduino Library For Reading Analog Voltage And Calculating Pressure From The Danfoss MBS1250 Sensor (10Bar Varient).

## What's New In v0.6.0

- Ability To Monitor System Vcc Usin Internal 1.1V  Reference

## Full Function Reference

| Method						| Description
|-------------------------------|-----------------------------------------------------------
| 'begin()'						| Optional Arduino-Style Init
| 'readVoltage()'				| Returns Clamped Voltage In Volts (Default 0.5-4.5V)
| 'readRawPressure()'			| Returns Pressure In Bar Using Unclamped Voltage
| 'readPressure(unit)'			| Returns Pressure In '"BAR"' (Default, '"psi"', Or '"kPa"'
| 'enableClamping(true/false)'	| Turns Voltage Clamping On Or Off
| 'getPressureMin()'			| Returns Sensor's Minimum Pressure
| 'getPressureMax()'			| Returns Sensor's Maximum Pressure
| 'isPressureOutOfRange()'		| Returns True If Voltage is Outside 0.45-4.55V
| 'getSupplyVoltage()'			| Measures Vcc Using Internal 1.1V Reference(AVR Only)

