#include <SoftwareSerial.h>
//load libraries
#include <LiquidCrystal_I2C.h>

#include <ArduinoJson.h>

LiquidCrystal_I2C      lcd(0x27, 16, 2);

SoftwareSerial SERIAL_ESP(7,8); // RX, TX

void setup() {
  Serial.begin(9600);
  Serial.println("hello");
  SERIAL_ESP.begin(9600);
  // The begin call takes the width and height. This
  // Should match the number provided to the constructor.
  lcd.begin(16,2);
  lcd.init();
  // Turn on the backlight.
  lcd.backlight();
  lcd.clear();
}

unsigned long timeReadSerial;
#define TIME_OUT_READ_SERIAL 1000
String stringReceiveFromSlave = "";
#define TEMP 0
#define HUMI 1
#define RAIN 2
#define GAS 3
uint8_t statusDevices[4]={};
void loop() {
  if(SERIAL_ESP.available() > 0) {
    char cmd = SERIAL_ESP.read();
    if(cmd == 0x05) {
      Serial.println("update sim");
    }else if(cmd == '{'){
      stringReceiveFromSlave = "";
      stringReceiveFromSlave += cmd;
      timeReadSerial = millis();
      while ((millis() - timeReadSerial < TIME_OUT_READ_SERIAL)) {
        if(SERIAL_ESP.available()>0) {
          char cmd = SERIAL_ESP.read();
          stringReceiveFromSlave+=cmd;
          if(cmd == '}') {
            DynamicJsonBuffer jsonBuffer;
            JsonObject& root = jsonBuffer.parseObject(stringReceiveFromSlave);
            if (root.success()) {
              statusDevices[TEMP] = root["TEMP"];
              statusDevices[HUMI] = root["HUMI"];
              statusDevices[RAIN] = root["RAIN"];
              statusDevices[GAS] = root["GAS"];
            }     
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("TEM:");
            lcd.print(String(statusDevices[TEMP]));
            lcd.setCursor(8, 0);
            lcd.print("HUM:");
            lcd.print(String(statusDevices[HUMI]));
            lcd.setCursor(0, 1);
            lcd.print("RAI:");
            lcd.print(String(statusDevices[RAIN]));
            lcd.setCursor(8, 1);
            lcd.print("GAS:");
            lcd.print(String(statusDevices[GAS]));
            break;
          }
        }
      }
    }
  }
}
