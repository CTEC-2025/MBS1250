# MBS1250 Arduino Library

### 🆕 What's New in v1.3.0
- 🛫 **Auto-Zero on Startup**:  
  Use `enableAutoZero(true)` to automatically zero the sensor during `begin()`, perfect for offset drift removal at power-up.  
- 🌎 **Expanded Pressure Unit Support**:  
  Now supports `"bar"`, `"psi"`, `"kPa"`, `"atm"`, and `"mmHg"` outputs in all pressure reading functions.
- 🧹 **Internal Improvements**:  
  Cleaner codebase, unified pressure unit conversion helper.

---

## Function Reference

| Method                            | Description                                                     |
|-----------------------------------|-----------------------------------------------------------------|
| `begin()`                         | Initializes the sensor and performs board-specific checks.      |
| `readVoltage()`                   | Returns clamped sensor voltage in volts (based on calibration). |
| `readRawPressure()`               | Returns raw pressure calculated from the sensor voltage.        |
| `readPressure(unit)`              | Returns pressure in requested unit ("bar", "psi", "kPa", "atm", "mmHg"). |
| `readSmoothedPressure(n, unit)`   | Averages multiple pressure readings for noise reduction.        |
| `setZeroOffset(offsetBar)`        | Adjusts the baseline pressure reading.                          |
| `setCalibration(vMin, vMax, pMin, pMax)` | Sets a custom calibration mapping.                       |
| `resetCalibration()`              | Resets calibration parameters to default.                       |
| `saveCalibrationToEEPROM()`       | Store calibration and offset to EEPROM.   |
| `loadCalibrationFromEEPROM()`      | Load saved calibration and offset from EEPROM. |
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
| `printCalibration()`              | Prints current voltage/pressure calibration values     |
| `setSmoothingMode(mode)`           | Sets smoothing mode (none, average, EMA)               |
| `readPressureSmoothed(unit)`       | Reads pressure with selected smoothing mode           |
| `getSensorStatus()`                | Returns sensor status (OK, clamped, disconnected, etc.) |
| `getReading(unit)`                 | Returns full structured snapshot of sensor values     |
| `enableAutoZero(on)`              | Enables/disables auto-zeroing on startup. (v1.3.0)              |

---

### Supported Units for Pressure Readings
| Unit  | Description                   |
|:------|:------------------------------|
| bar   | Standard output (default)      |
| psi   | Pounds per square inch         |
| kPa   | Kilopascals                    |
| atm   | Atmospheres                    |
| mmHg  | Millimeters of mercury (Torr)  |

---

## 📦 Installation

### Option 1: Arduino Library Manager
- Open the Arduino IDE.
- Go to **Tools > Manage Libraries...**
- Search for `MBS1250` and click Install.

### Option 2: Manual Installation
- Download this repository as a `.zip`.
- Go to **Sketch > Include Library > Add .ZIP Library...**.
- Select the downloaded archive.

---

## 📚 Examples Included

| Example Sketch         | Description                                   |
|-------------------------|-----------------------------------------------|
| BasicRead               | Quick start with default settings.           |
| AdvancedRead            | Read voltage, pressure, and unit conversions.|
| SmoothedDiagnostics     | Averaged readings with fault detection.       |
| CalibrationEEPROM       | Save/load custom calibration to EEPROM.      |
| FullFeatureDemo         | Full showcase with smoothing and status.     |

---

🛠 Built for robust real-world projects.  
Now featuring **auto-zero** and **expanded unit flexibility**! 🚀