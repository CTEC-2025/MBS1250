#include <MBS1250.h>

MBS1250 sensor(A0, 5.0);  // Analog pin A0, 5V reference

void setup() {
  Serial.begin(9600);
  delay(100);
  Serial.println(F("MBS1250 Full Feature Demo – v1.4.0"));

  sensor.begin();

  // Enable Auto-Zero at Startup
  sensor.enableAutoZero(true);

  // Enable Debug Mode
  sensor.enableDebug(true);

  // Set smoothing mode (choose one!)
  sensor.setSmoothingMode(SMOOTH_EMA); // Options: SMOOTH_NONE, SMOOTH_AVERAGE, SMOOTH_EMA

  // (Optional: direct EMA smoothing setup legacy style)
  // sensor.enableEMASmoothing(true, 0.15);
}

void loop() {
  Serial.println(F("\n--- Sensor Full Data Snapshot ---"));

  // Read pressure using smoothing mode
  float smoothedPressureBar = sensor.readPressureSmoothed("bar");
  float smoothedPressurePsi = sensor.readPressureSmoothed("psi");

  Serial.print(F("Smoothed Pressure: "));
  Serial.print(smoothedPressureBar, 2);
  Serial.println(F(" bar"));

  Serial.print(F("Smoothed Pressure: "));
  Serial.print(smoothedPressurePsi, 2);
  Serial.println(F(" psi"));

  // Full structured reading
  PressureData data = sensor.getReading("bar");
  Serial.print(F("Raw Voltage: "));
  Serial.print(data.voltage, 3);
  Serial.println(F(" V"));

  Serial.print(F("Pressure (struct): "));
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
      Serial.println("OK");
      break;
    case SENSOR_DISCONNECTED:
      Serial.println("DISCONNECTED");
      break;
    case SENSOR_CLAMPED:
      Serial.println("CLAMPED");
      break;
    case SENSOR_OUT_OF_RANGE:
      Serial.println("OUT OF RANGE");
      break;
    default:
      Serial.println("UNKNOWN");
      break;
  }

  // New v1.4.0 Features
  Serial.print(F("Pressure Rate (bar/sec): "));
  Serial.println(sensor.getPressureRate(), 3);

  Serial.print(F("Peak Pressure Recorded: "));
  Serial.print(sensor.getPeakPressure(), 2);
  Serial.println(F(" bar"));

  Serial.print(F("Lowest Pressure Recorded: "));
  Serial.print(sensor.getLowestPressure(), 2);
  Serial.println(F(" bar"));

  delay(1500);
}
