#include <MBS1250.h>

MBS1250 sensor(A0, 5.0);  // Analog pin A0, 5V reference

void setup() {
  Serial.begin(9600);
  delay(500);
  Serial.println(F("MBS1250 Full Feature Demo – v1.1.0"));

  sensor.begin();

  // Optional: apply custom calibration
  // sensor.setCalibration(0.5, 4.5, 0.0, 10.0);

  // Enable EMA smoothing (optional)
  sensor.enableEMASmoothing(true, 0.15);

  // Turn on debug output (optional)
  sensor.enableDebug(true);
}

void loop() {
  Serial.println(F("\n--- Sensor Data ---"));

  // Raw voltage read
  float voltage = sensor.readVoltage();
  Serial.print(F("Clamped Voltage: "));
  Serial.print(voltage, 3);
  Serial.println(" V");

  // Pressure in bar with zero offset
  float pressureBar = sensor.readPressure("bar");
  Serial.print(F("Pressure (bar): "));
  Serial.print(pressureBar, 2);
  Serial.println(" bar");

  // Pressure in psi
  Serial.print(F("Pressure (psi): "));
  Serial.print(sensor.readPressure("psi"), 2);
  Serial.println(" psi");

  // Smoothed pressure (average of 10 samples)
  float avgPressure = sensor.readSmoothedPressure(10, "bar");
  Serial.print(F("Smoothed Avg (bar): "));
  Serial.println(avgPressure, 2);

  // EMA smoothing
  float ema = sensor.readPressureEMA("bar");
  Serial.print(F("EMA Smoothed (bar): "));
  Serial.println(ema, 2);

  // Clamping diagnostic
  if (sensor.isClamped()) {
    Serial.println(F("[Warning] Voltage was clamped during last read."));
  }

  // Sensor connection check
  if (!sensor.isSensorConnected()) {
    Serial.println(F("[Error] Sensor may be disconnected!"));
  }

  // Voltage out-of-range diagnostic
  if (sensor.isPressureOutOfRange()) {
    Serial.println(F("[Warning] Voltage out of expected range!"));
  }

  // Access last values
  Serial.print(F("Last Voltage: "));
  Serial.println(sensor.getLastVoltage(), 3);
  Serial.print(F("Last Pressure: "));
  Serial.println(sensor.getLastPressure(), 2);

  delay(1500);
}