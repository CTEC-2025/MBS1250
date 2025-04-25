#include <MBS1250.h>

MBS1250 sensor(A0, 5.0);  // Analog pin A0, 5V reference

void setup() {
  Serial.begin(9600);
  delay(100);
  Serial.println(F("MBS1250 Full Feature Demo – v1.2.5"));

  sensor.begin();

  // Apply custom calibration if needed
  // sensor.setCalibration(0.5, 4.5, 0.0, 10.0);

  // Enable Debug Mode
  sensor.enableDebug(true);

  // Set unified smoothing mode (choose one!)
  sensor.setSmoothingMode(SMOOTH_EMA); // Options: SMOOTH_NONE, SMOOTH_AVERAGE, SMOOTH_EMA

  // (Optional: direct EMA legacy smoothing setup)
  // sensor.enableEMASmoothing(true, 0.15);
}

void loop() {
  Serial.println(F("\n--- Sensor Full Data Snapshot ---"));

  // Read pressure using smoothing mode
  float smoothedPressure = sensor.readPressureSmoothed("bar");
  Serial.print(F("Smoothed Pressure (bar): "));
  Serial.println(smoothedPressure, 2);

  // Full structured reading
  PressureData data = sensor.getReading("bar");
  Serial.print(F("Raw Voltage: "));
  Serial.print(data.voltage, 3);
  Serial.println(F(" V"));

  Serial.print(F("Pressure: "));
  Serial.print(data.pressure, 2);
  Serial.println(F(" bar"));

  Serial.print(F("Sensor Connected: "));
  Serial.println(data.connected ? "YES" : "NO");

  Serial.print(F("Clamped: "));
  Serial.println(data.clamped ? "YES" : "NO");

  // Sensor status check
  SensorStatus status = sensor.getSensorStatus();
  Serial.print(F("Sensor Status: "));
  switch (status) {
    case SENSOR_OK:
      Serial.println(F("OK"));
      break;
    case SENSOR_DISCONNECTED:
      Serial.println(F("DISCONNECTED"));
      break;
    case SENSOR_CLAMPED:
      Serial.println(F("CLAMPED"));
      break;
    case SENSOR_OUT_OF_RANGE:
      Serial.println(F("OUT OF RANGE"));
      break;
    default:
      Serial.println(F("UNKNOWN"));
      break;
  }

  delay(1500);
}