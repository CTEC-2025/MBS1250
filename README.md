# MBS1250 Arduino Library

🎉 v1.0.0 – First Stable Release
The MBS1250 Arduino Library is now complete, tested, and stable!

## Features In V1.0.0

- ✅ Voltage-to-pressure conversion (0.5–4.5V default)
- ✅ Unit output in **bar**, **psi**, or **kPa**
- ✅ Custom calibration: `setCalibration(vMin, vMax, pMin, pMax)`
- ✅ Zero offset adjustment
- ✅ Pressure smoothing via averaging
- ✅ Sensor diagnostics (disconnection + out-of-range)
- ✅ Supply voltage measurement (AVR only)
- ✅ EEPROM save/load of calibration and offset
- ✅ Clean, Arduino-friendly API

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

### Supported Hardware

| Platform							| Notes
|-----------------------------------|-----------------------------------------------------------------
| Arduino Uno/Nano					| Fully Supported, Including getSupplyVoltage()
| ESP32/ESP8266						| Fully Supported Except For Supply Voltage
| Other Boards						| Works If analogRead() is Supported


## 📦 Installation

### Option 1: Library Manager (coming soon)
- Open the Arduino IDE
- Go to **Tools > Manage Libraries...**
- Search for `MBS1250` and click Install

### Option 2: Manual
- Download this repository as a `.zip`
- Go to **Sketch > Include Library > Add .ZIP Library...**
- Select the downloaded archive
