# MBS1250 Arduino Library

### 🔄 New in v1.1.0

- 🔁 **EMA Smoothing**: Enable exponential moving average filtering with `enableEMASmoothing()` and read with `readPressureEMA()`.
- 📉 **Clamping Detection**: Use `isClamped()` to check if the last voltage was limited to calibration bounds.
- 🧾 **Last Value Tracking**: Retrieve the last voltage or pressure reading using `getLastVoltage()` and `getLastPressure()`.
- 🪛 **Debug Mode**: Enable debug Serial output with `enableDebug(true)` for development insights.

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
| `enableEMASmoothing(enabled, alpha)` | Enables EMA smoothing and sets smoothing factor     		  |
| `readPressureEMA(unit)`              | Returns pressure with EMA filtering applied          		  |
| `isClamped()`                        | Returns `true` if the last read voltage was clamped  		  |
| `getLastVoltage()`                  | Returns the most recent sensor voltage               		  |
| `getLastPressure()`                 | Returns the most recent pressure (in bar)            		  |
| `enableDebug(true/false)`           | Turns on/off internal debug print statements         		  |

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
