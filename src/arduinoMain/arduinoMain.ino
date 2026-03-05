#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include "TH02_dev.h"
#include <math.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(115200);
  Wire.begin();
  TH02.begin();

  // init lcd 
  lcd.init();
  lcd.backlight();
}

void loop() {
  float temp = TH02.ReadTemperature();
  float humidity = TH02.ReadHumidity();
  int lightValue = analogRead(A0); // plugged in the light sensor to A0
  // Calculate resistance of sensor in K
  // float resistance = (1023.0 - lightValue) * 10.0 / lightValue;
  // float lux = 325*pow(resistance, -1.4);

  delay(1000);

  // set values to lcd
  lcd.setCursor(0,0);
  lcd.print("temp=");
  lcd.print((float)temp);
  lcd.print("C");

    if ( temp < 20 ){
    lcd.print(" COLD");
  } else if (temp >= 20 && temp <= 22 ){
    lcd.print(" :)");
  } else {
    lcd.print(" HOT");
  }
  
  lcd.setCursor(0,1);
  lcd.print("humi=");
  lcd.print((float)humidity);
  lcd.print("%");

  if ( humidity > 40 ){
    lcd.print(" :)");
  } else {
    lcd.print(" :(");
  }

  
  
  
  // send data in CSV format: temperature, humidity, resistance
  Serial.print(temp, 2);
  Serial.print(",");
  Serial.print(humidity, 2);
  Serial.print(",");
  Serial.println(lightValue);

  delay(1000);
}
