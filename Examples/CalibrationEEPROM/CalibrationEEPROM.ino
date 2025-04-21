#include <MBS1250.h>

MBS1250 sensor(A0, 5.0);

void setup() {
  Serial.begin(9600);
  sensor.begin();

  // Uncomment this to apply and save calibration:
  // sensor.setCalibration(0.5, 4.5, 0.0, 10.0);
  // sensor.setZeroOffset(-0.1);
  // sensor.saveCalibrationToEEPROM();

  sensor.loadCalibrationFromEEPROM();
}

void loop() {
  float pressure = sensor.readPressure("bar");
  Serial.print("Restored Calibrated Pressure: ");
  Serial.print(pressure, 2);
  Serial.println(" bar");

  delay(1000);
}