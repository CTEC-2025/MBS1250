# MBS1250 Arduino Library

### 🆕 What's New in v1.4.0
- 📈 **Pressure Rate of Change Calculation**:  
  New `getPressureRate()` method lets you monitor how quickly pressure rises or falls (in bar/sec, psi/sec, etc.).
  
- 🏔️ **Peak and Lowest Pressure Tracking**:  
  - `getPeakPressure()` returns the highest recorded pressure.
  - `getLowestPressure()` returns the lowest recorded pressure.
  - `resetPeakHold()` resets the peak and low memory.

- 🛹 **Internal Improvements**:  
  - Smoother tracking of live pressure changes.
  - Core ready for even more advanced features in future versions!

---

## Function Reference

| Method                            | Description                                                     |
|-----------------------------------|-----------------------------------------------------------------|
| `begin()`                         | Initializes the sensor and performs board-specific checks.      |
| `enableAutoZero(on)`              | Enables/disables auto-zeroing on startup.                        |
| `readVoltage()`                   | Returns clamped sensor voltage (in volts).                       |
| `readRawPressure()`               | Returns raw pressure calculated from the sensor voltage.         |
| `readPressure(unit)`              | Returns pressure in requested unit ("bar", "psi", "kPa", "atm", "mmHg"). |
| `readSmoothedPressure(n, unit)`   | Averages multiple pressure readings for noise reduction.         |
| `enableClamping(on)`              | Enables voltage clamping within calibration range.               |
| `setZeroOffset(offsetBar)`        | Adjusts baseline pressure manually.                              |
| `setCalibration(vMin, vMax, pMin, pMax)` | Sets custom voltage-to-pressure mapping.                   |
| `resetCalibration()`              | Resets calibration parameters to defaults.                       |
| `saveCalibrationToEEPROM()`       | Stores calibration and offset into EEPROM.                       |
| `loadCalibrationFromEEPROM()`     | Loads calibration and offset from EEPROM.                        |
| `enableEMASmoothing(enabled, alpha)` | Enables EMA smoothing and sets smoothing factor.             |
| `setSmoothingMode(mode)`          | Sets smoothing mode (NONE, AVERAGE, EMA).                         |
| `readPressureEMA(unit)`           | Reads pressure using EMA filtering.                               |
| `readPressureSmoothed(unit)`      | Reads pressure with selected smoothing mode.                      |
| `getSensorStatus()`               | Returns sensor status (OK, disconnected, clamped, out-of-range).  |
| `getReading(unit)`                | Returns full structured snapshot of sensor values.                |
| `isClamped()`                     | Returns `true` if last voltage reading was clamped.                |
| `getLastVoltage()`                | Returns most recent sensor voltage reading.                       |
| `getLastPressure()`               | Returns most recent pressure (in bar).                             |
| `enableDebug(on)`                 | Enables or disables internal debug print statements.              |
| `isPressureOutOfRange()`          | Checks if voltage is outside calibration range (±0.05V margin).    |
| `isSensorConnected()`             | Checks if sensor appears properly connected.                      |
| `getSupplyVoltage()`              | Returns board supply voltage (AVR-only feature).                   |
| `getPressureMin()/getPressureMax()` | Returns calibrated pressure range.                                |
| `getVoltageMin()/getVoltageMax()`  | Returns calibrated voltage range.                                 |
| `getPressureRate()`               | Returns live pressure rate of change (bar/sec, psi/sec, etc.).     |
| `getPeakPressure()`               | Returns highest pressure recorded.                                |
| `getLowestPressure()`             | Returns lowest pressure recorded.                                 |
| `resetPeakHold()`                 | Resets peak and lowest pressure memory.                           |

---

### Supported Units for Pressure Readings

| Unit  | Description                   |
|:------|:------------------------------|
| bar   | Standard output (default).     |
| psi   | Pounds per square inch.        |
| kPa   | Kilopascals.                   |
| atm   | Atmospheres.                   |
| mmHg  | Millimeters of mercury (Torr). |

---

## 📦 Installation

### Option 1: Arduino Library Manager (in progress)
- Open the Arduino IDE.
- Go to **Tools > Manage Libraries...**
- Search for `MBS1250` and click Install.

### Option 2: Manual Installation
- Download this repository as a `.zip`.
- Go to **Sketch > Include Library > Add .ZIP Library...**
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

🛠 Built for real-world projects.  
Now featuring **auto-zero**, **expanded unit support**, **pressure rate tracking**, and **peak pressure memory**! 🚀
