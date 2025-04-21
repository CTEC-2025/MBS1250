# MBS1250 Arduino Library

## Version: 0.9.0 ##
A Lightweight Arduino Library For Reading Analog Voltage And Calculating Pressure From The Danfoss MBS1250 Sensor (10Bar Varient).

## What's New In v0.9.0

- Added EEPROM Save/Load Support For Calibration + Zero Offset
- Struct-Based, Checksum-Verified Storage
- New Example : CalibrationEEPROM.ino
- Smoothed Pressure Guards(Samples Limited to 1-100)
- Final Polish And Ready For V1.0.0

## Function Reference

| Method                            | Description                                                     |
|-----------------------------------|-----------------------------------------------------------------|
| `begin()`                         | Initializes the sensor and performs board-specific checks.      |
| `readVoltage()`                   | Returns clamped sensor voltage in volts (based on calibration). |
| `readRawPressure()`               | Returns raw pressure calculated from the sensor voltage.        |
| `readPressure(unit)`              | Returns pressure in the requested unit ("bar", "psi", "kPa").   |
| `readSmoothedPressure(n, unit)`   | Averages multiple pressure readings for noise reduction.        |
| `setZeroOffset(offsetBar)`        | Adjusts the baseline pressure reading.                          |
| `setCalibration(vMin, vMax, pMin, pMax)` | Sets a custom calibration mapping.                       |
| `resetCalibration()`              | Resets calibration parameters to default.                       |
| 'saveCalibraionToEEPROM()' 		| Store Calibration + Offset In EEPROM							  |
| 'loadCalibrationFromEEPROM()'     | Load Saved Values From EEPROM (If Checksum Is Valid)			  |
| `isPressureOutOfRange()`          | Checks if the measured voltage is outside calibrated range (with a ±0.05V margin). |
| `isSensorConnected()`             | Determines if the sensor appears to be properly connected.      |
| `getSupplyVoltage()`              | Returns the board supply voltage (AVR only).                    |
| `getPressureMin()/getPressureMax()` | Returns the calibrated pressure range.                        |
| `getVoltageMin()/getVoltageMax()`   | Returns the calibrated voltage range.                         |

...

## 📦 Installation

1. Download or clone this repository.
2. Place the folder in your Arduino `libraries/` directory.
3. Restart the Arduino IDE or select **Sketch > Include Library > Add .ZIP Library**.