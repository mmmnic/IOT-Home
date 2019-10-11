#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>
#include <ArduinoJson.h>
LiquidCrystal_I2C      lcd(0x27, 16, 2);
SoftwareSerial SERIAL_ESP(7,8); // RX, TX
SoftwareSerial SIM_SERIAL(3, 2); // RX, TX

//sim init

void simInit() {
  SIM_SERIAL.println("ATE0");                     // Tat che do phan hoi (Echo mode)
  delay(2000);
  SIM_SERIAL.println("AT+IPR=9600");              // Dat toc do truyen nhan du lieu 9600 bps
  delay(2000);
  SIM_SERIAL.println("AT+CMGF=1");                // Chon che do TEXT Mode
  delay(2000);
  SIM_SERIAL.println("AT+CLIP=1");                // Hien thi thong tin nguoi goi den
  delay(2000);
  SIM_SERIAL.println("AT+CNMI=2,2");              // Hien thi truc tiep noi dung tin nhan
  delay(2000);
}

void simCall(String phoneNum) {
  Serial.println("call");
  SIM_SERIAL.println("ATD" + phoneNum + ";");       // Goi dien 
  delay(20000);                                // Sau 15s
  SIM_SERIAL.println("ATH");                       // Ngat cuoc goi
  delay(2000);
}

void simSendSMS(String phoneNum, String content) {
  Serial.println("sending sms");
  SIM_SERIAL.println("AT+CMGS=\"" + phoneNum + "\"");
  delay(3000);
  SIM_SERIAL.print(content);
  SIM_SERIAL.print((char)26);
  delay(3000);           
}

String messageSaved = "";

void checkSMS() {
  if (SIM_SERIAL.available() > 0) {
    unsigned long timeOutCheck = millis();
    String stringReceived = "";
    uint8_t stepProcess = 0; //chua gi ca
    while (millis() - timeOutCheck < 1000) {
      if (SIM_SERIAL.available() > 0) {
        char chReceived = SIM_SERIAL.read();
        if (chReceived == 0x0D && stepProcess == 0) {
          stepProcess = 1;
          continue;
        }
        if (chReceived == 0x0A && stepProcess == 1) {
          stepProcess = 2;
          break;
        }
        if (stepProcess == 0) {
          stringReceived += chReceived;
        }
      }
    }
    if(stepProcess != 2) {
      Serial.println("fail read SMS");
      return;
    } else {
      if(stringReceived.indexOf("+CMT:") >= 0) {
          stringReceived = "";
          timeOutCheck = millis();
          stepProcess = 0;
          while (millis() - timeOutCheck < 1000) {
            if (SIM_SERIAL.available() > 0) {
              char chReceived = SIM_SERIAL.read();
              if (chReceived == 0x0D && stepProcess == 0) {
                stepProcess = 1;
                continue;
              }
              if (chReceived == 0x0A && stepProcess == 1) {
                stepProcess = 2;
                break;
              }
              if (stepProcess == 0) {
                stringReceived += chReceived;
              }
            }
          }
          if(stepProcess != 2) {
            Serial.println("fail read SMS");
            return;
          } else {
            Serial.print("SMS content: ");
            Serial.println(stringReceived);
            messageSaved+=(stringReceived + (char)0x01);
            return;
          }
      } else {
        if(stringReceived == "OK" || stringReceived== "") {
          return;
        }
        Serial.println("this is not sms sign");
      }
    }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);
  Serial.println();Serial.println();Serial.println();
  SIM_SERIAL.begin(9600);
  simInit();
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
  //========================================================================================
  //                                        sms
  //========================================================================================
  checkSMS();
  //========================================================================================
  //                                        lcd-esp
  //========================================================================================
  if(SERIAL_ESP.available() > 0) {
    char cmd = SERIAL_ESP.read();
    if(cmd == 0x05) {
      Serial.println("update sim");
      SERIAL_ESP.print("{\"Mess\":\""+messageSaved+"\"}");
      messageSaved="";
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
