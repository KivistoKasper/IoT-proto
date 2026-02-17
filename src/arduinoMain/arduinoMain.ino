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
  int lightValue = analogRead(A0); // plugged in the light sensor to A0

  delay(1000);
  
  // send data in CSV format: temperature, humidity, light value
  Serial.print(temp, 2);
  Serial.print(",");
  Serial.print(humidity, 2);
  Serial.print(",");
  Serial.println(lightValue);

  delay(1000);
}
