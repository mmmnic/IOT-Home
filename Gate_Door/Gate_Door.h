#include <EEPROM.h>
#include <MFRC522.h> 
#include <ArduinoJson.h>
#include <SoftwareSerial.h>
#include <Keypad.h>
#include <String.h>
#include <SPI.h>
#include <Servo.h> 
#include <LiquidCrystal_I2C.h>
#include <Wire.h>


/*
//Cách nối chân RFID
 * Reset      9
 * SPI SDA    10
 * SPI MOSI   11
 * SPI MISO   12
 * SPI SCK    13
//Cách nối chân keypad
 * Hàng {3, 4, A5 , A4};
 * Cột = {A0, A1, A2, A3};
//Cách nối chân LCD
 * SDA 		  A4
 * SCL 		  A5
*/

// define Pin
#define SS_PIN 10
#define RST_PIN 9
#define light 4
#define door 5
#define gate 6
#define soft_RX 8
#define soft_TX 7
#define TIME_OUT_READ_SERIAL 1000

  
// khai báo thẻ
int cardID1[4] = {93,   1, 234, 169};
int cardID2[4] = {233, 44, 163,  43};

// định nghĩa keypad
const byte rows = 4; //số hàng
const byte columns = 4; //số cột

// Định nghĩa các pin cho keypad
byte rowPins[rows] = {0, 1, 2, 3};			
byte columnPins[columns] = {17, 16, 15, 14};

//Định nghĩa các giá trị trả về
char keys[rows][columns] =
{
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'},
};

// biến toàn cục
int uidDec[4];  					      // biến lưu giá trị của thẻ quẹt
char *gateStatus = "0";				  // biến lưu trạng thái cổng
char *lightStatus = "0"; 			  // biến lưu trạng thái cửa
char *doorStatus = "0";				  // biến lưu trạng thái đèn
const char* receivedJson ="";		// biến chuỗi json nhận được
const char* COMMAND;				    // biến lưu COMMAND
const char* ID;						      // biến lưu ID
unsigned long myTime;           // biến mytime để delay
int firstPress = 1;             // biến kiểm tra lần bấm keypad đầu tiên
int isPassword = 0;             // biến kiểm tra mật khẩu đúng hay sai  
char keypadData = '0';          // biển lưu nút bấm của keypad
// mật khẩu 1
String keypadPass1 = "0000";
int pass1Addr = 4;
// mật khẩu 2
String keypadPass2 = "0000";
int pass2Addr = 8;
// mật khẩu 3
String keypadPass3 = "0000";
int pass3Addr = 12;
// mật khẩu 4
String keypadPass4 = "0000";
int pass4Addr = 16;
