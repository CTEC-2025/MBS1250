# MBS1250 Arduino Library

## Version: 0.8.0 ##
A Lightweight Arduino Library For Reading Analog Voltage And Calculating Pressure From The Danfoss MBS1250 Sensor (10Bar Varient).

## What's New In v0.8.0

- **Custom Calibration:** 
  - `setCalibration(vMin, vMax, pMin, pMax)` to map any sensor's voltage range to the corresponding pressure.
  - `resetCalibration()` to restore default values (0.5V–4.5V → 0–10 bar).
- **Zero Offset Calibration:** 
  - `setZeroOffset(offsetBar)` allows fine-tuning of the pressure output.
- Added getters for the voltage range: `getVoltageMin()` and `getVoltageMax()`.
- The pressure conversion now uses the customizable calibration parameters.
- Extended documentation and updated examples.

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
| `isPressureOutOfRange()`          | Checks if the measured voltage is outside calibrated range (with a ±0.05V margin). |
| `isSensorConnected()`             | Determines if the sensor appears to be properly connected.      |
| `getSupplyVoltage()`              | Returns the board supply voltage (AVR only).                    |
| `getPressureMin()/getPressureMax()` | Returns the calibrated pressure range.                        |
| `getVoltageMin()/getVoltageMax()`   | Returns the calibrated voltage range.                         |

...

## Installation

1. Clone or download this repository.
2. Place the folder into your Arduino `libraries/` directory.
3. Restart the Arduino IDE.