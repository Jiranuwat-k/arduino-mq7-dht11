#include "MQ7.h"
#include "DHT.h"
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <SPI.h>  
#include <RTClib.h>

void dht11_page();
void mq7_page();

#define A_PIN A0
#define VOLTAGE 5

// init MQ7 device
RTC_DS3231 RTC;
LiquidCrystal_I2C lcd(0x27,16,2);
MQ7 mq7(A_PIN, VOLTAGE);
DHT dht;
void setup() {
  Serial.begin(9600);
  lcd.backlight();
  lcd.init();
  Wire.begin();
  RTC.begin();
  pinMode(3,INPUT_PULLUP);
//  while (!Serial) {
//    ; // wait for serial connection
//  }
  if (! RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
    RTC.adjust(DateTime(__DATE__, __TIME__));
  }
  
  dht.setup(2);
  Serial.println("Calibrating MQ7");
  lcd.setCursor(2,0);
  lcd.print("Calibrating.");
  mq7.calibrate();    // calculates R0
  Serial.println("Calibration done!");
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Calibration done!");
  delay(500);
  lcd.clear();
}
void loop(){
  dht11_page();
}
void dht11_page(){
      while(1){
      float humi = dht.getHumidity();
      float temp = dht.getTemperature();
      delay(dht.getMinimumSamplingPeriod());
      Serial.print("temperature = ");Serial.println(temp);
      Serial.print("humidity = ");Serial.println(humi);
      Serial.print(dht.getMinimumSamplingPeriod());
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("temp : ");
      lcd.print(temp);
      lcd.setCursor(13,0);
      lcd.print("^c");
      lcd.setCursor(0,1);
      lcd.print("humidity : ");
      lcd.print(humi);
      if(digitalRead(3) == 1){
          mq7_page();
      }
      }
}
void mq7_page(){
      while(1){
      int co = mq7.readPpm();
      Serial.print("CO : PPM = "); Serial.println(co);
      lcd.clear();
      lcd.setCursor(1,0);
      lcd.print("CO : "); 
      lcd.setCursor(6,0);
      lcd.print(co);
      lcd.setCursor(12,0);
      lcd.print("PPM");
      delay(1000);
      if (digitalRead(3) == 0){
        dht11_page();
      }
      }
}
