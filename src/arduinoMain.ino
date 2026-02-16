#include <Wire.h>
#include "TH02_dev.h"

void setup() {
  Serial.begin(9600);
  Wire.begin();
  TH02.begin();
}

void loop() {
  float temp = TH02.ReadTemperature();
  float humidity = TH02.ReadHumidity();

  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.print(" °C  Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  delay(2000);
}
