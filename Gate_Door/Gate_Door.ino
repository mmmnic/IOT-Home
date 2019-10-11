#include "Gate_Door.h"

// khai báo Serial giao tiếp với nodeMCU
SoftwareSerial mySerial(soft_RX, soft_TX);
// cài đặt keypad
Keypad keypad = Keypad(makeKeymap(keys), rowPins, columnPins, rows, columns);
// khai báo spi cho rfid
MFRC522 mfrc522(SS_PIN, RST_PIN);
// cấu hình địa chỉ lcd là 0x27 và 16 cột 2 hàng
LiquidCrystal_I2C lcd(0x27, 16, 2);

//khởi tạo servo
Servo SVdoor;
Servo SVgate;

void setup() {
  // cấu hình pin 5 cho cửa
  pinMode(door, OUTPUT);
  // cấu hình pin 6 cho cổng
  pinMode(gate, OUTPUT);
  // cấu hình pin 4 cho đèn
  pinMode(light, OUTPUT);
  // tắt đèn
  digitalWrite(light, HIGH);

  // cấu hình servo cho cửa và cổng
  SVdoor.attach(door);
  SVgate.attach(gate);

  // khởi động SPI
  SPI.begin();
  // cấu hình RFID
  mfrc522.PCD_Init();

  // mở cổng serial với nodeMCU
  mySerial.begin(9600);

  // khởi động LCD
  lcd.begin();
  // bật đèn nền LCD
  lcd.backlight();


  // xóa dấu /* và */ để đặt mật khẩu lại 0000
  /*
  for (int i = 0; i<=16; i++)
  {
    EEPROM.write(i, '0');
    delay(5);
  }  
  */

  // lấy mật khẩu từ bộ nhớ eeprom và lưu vào các biến
  for (int i = 0; i<pass1Addr; i++)
  {
    keypadPass1[i] = char (EEPROM.read(i));
  }
  for (int i = pass1Addr; i<pass2Addr; i++)
  {
    keypadPass2[i-pass1Addr] = char (EEPROM.read(i));
  }
  for (int i = pass2Addr; i<pass3Addr; i++)
  {
    keypadPass3[i-pass2Addr] = char (EEPROM.read(i));
  }
  for (int i = pass3Addr; i<pass4Addr; i++)
  {
    keypadPass4[i-pass3Addr] = char (EEPROM.read(i));
  }
}


void loop() {
  // xóa dữ liệu trong biến ID, COMMAND và password
  ID = "";
  COMMAND = "";
  isPassword = 0;

  // hàm nhận và giải mã chuỗi JSON
  parseJson();
  // so sánh kết quả và thực thi
  if (!strcmp(ID, "CC"))
  {
    if (!strcmp(COMMAND, "1"))
    {
      openGate();
    }
    else if (!strcmp(COMMAND, "2"))
    {
      closeGate();
    }
    else if (!strcmp(COMMAND, "3"))
    {
      lightOn();
    }
    else if (!strcmp(COMMAND, "4"))
    {
      lightOff();
    }
    // gửi tình trạng lại cho server
    sendStatus(gateStatus, lightStatus, doorStatus);
  }
  else if (!strcmp(ID, "CN"))
  {
    if (!strcmp(COMMAND, "1"))
    {
      openDoor();
    }
    else if (!strcmp(COMMAND, "2"))
    {
      closeDoor();
    }
    // gửi tình trạng lại cho server
    sendStatus(gateStatus, lightStatus, doorStatus);
  }
  else if (!strcmp(COMMAND, "99"))
  {
    // gửi tình trạng lại cho server
    sendStatus(gateStatus, lightStatus, doorStatus);
  }


  // kiểm tra keypad
  getKeypad();
  // nếu đúng mật khẩu
  if(isPassword == 1)
  {
    // mở cửa
    openDoor();
  }

  // kiểm tra card nếu đúng mở cổng
  if (checkCard())
  {
    openGate();
  }

  // nếu cửa hoặc cổng mở quá 2s đóng của, hoặc cổng
  if (!strcmp(gateStatus, "1"))
  {
    // delay để cổng mở 2s
    myDelay(2000);
    closeGate();
  }
  if (!strcmp(doorStatus, "1"))
  {
    // delay để cửa mở 2s
    myDelay(2000);
    // đóng cửa mở 2s
    closeDoor();
  }
}

// hàm kiểm tra keypad
void getKeypad()
{
  // lưu ký tự nhập từ keypad 
  keypadData = keypad.getKey();
  delay(10);

  if (keypadData != NULL)
  {
    if (firstPress == 1)
    {
      // xóa màn hình nếu là lần nhập đầu
      lcd.clear();
      firstPress = 0;
    }
    // nếu phím nhập khác # là nhập mật khẩu
    if (keypadData != '#')
      checkPassword();
    // nếu là dấu # là đổi mật khẩu
    else if (keypadData == '#')
    {
      // kiểm tra nếu đổi mật khẩu thành công thì thoát vòng lặp
      while(!changePassword())
      {
        keypadData = keypad.getKey();
        delay(10);
      }
      myDelay(1000);
      lcd.clear();
      lcd.setCursor(3, 0);
      lcd.print("Cua da dong");
    }
  }
}

// hàm lấy mật khẩu
String getPassword()
{
  String tempPassword = "";
  lcd.setCursor(0, 0);
  lcd.print("  Nhap mat khau  ");
  // lặp đến khi nhập đủ 4 ký tự
  while (tempPassword.length() < 4)
  {
    char temp = keypad.getKey();
    delay(10);
    if (temp != NULL)
    {
      tempPassword += temp;
      lcd.setCursor(9, 1);
      // nếu nhập nút thì xóa mật khẩu
      if (temp == '*')
      {
        tempPassword = "";
        lcd.print("      ");
      }
      lcd.print(tempPassword);
    }
  }
  // return lại mật khẩu
  return tempPassword;
}

// hàm kiểm tra mật khẩu
int checkPassword()
{
  String keypadInputPass = getPassword();
  //nếu đủ 4 số
  if (keypadInputPass.length() == 4)
  {
    lcd.clear();
    //Serial.println();
    // kiểm tra có đúng với mật khẩu khai báo không
    if (keypadInputPass == keypadPass1 || keypadInputPass == keypadPass2 || keypadInputPass == keypadPass3 || keypadInputPass == keypadPass4)
    {
      keypadInputPass = "";
      firstPress = 1;
      isPassword = 1;
    }
    else
    {
      firstPress = 1;
      isPassword = 0;
      keypadInputPass = "";
      lcd.setCursor(3, 0);
      lcd.print("Sai mat khau ");
      myDelay(1000);
      lcd.clear();
      lcd.setCursor(3, 0);
      lcd.print("Cua da dong");
    }
  }
}

// Hàm đổi mật khẩu
int changePassword()
{
  // hàm hiện LCD
  lcd.setCursor(1,0);
  lcd.print("Chon tai khoan");
  
  delay(10);
  String keypadInputPass = "";
  // nếu tài khoản là A
  if (keypadData == 'A')
  {
    // LCD hiện chữ A
    lcd.clear();
    lcd.setCursor(2, 1);
    lcd.print("A: ");
    // lưu mật khẩu đã nhập vào biến
    keypadInputPass = getPassword();
    if (keypadInputPass.length() == 4)
    {
      // nếu đúng mật khẩu cũ tiến hành đổi mật khẩu
      if (keypadInputPass == keypadPass1)
      {
        lcd.clear();
        lcd.setCursor(2, 1);
        lcd.print("moi: ");
        // lưu mật khẩu vào biến
        keypadPass1 = getPassword();
        // lưu mật khẩu vào bộ nhớ eeprom
        for (int i = 0; i<pass1Addr; i++)
        {
            EEPROM.write(i, keypadPass1[i]);
            delay(5);
        }
        myDelay(500);
        lcd.clear();
        lcd.setCursor(3,0);
        lcd.print("hoan thanh");
        return 1;
      }
      // nếu sai thoát đổi mật khẩu
      else
      {
        keypadInputPass = "";
        lcd.setCursor(2, 0);
        lcd.print("Sai mat khau ");
        return 1;
      }
    }
  }
  // tương tự với A
  else if (keypadData == 'B')
  {
    lcd.clear();
    lcd.setCursor(2, 1);
    lcd.print("B: ");
    keypadInputPass = getPassword();
    if (keypadInputPass.length() == 4)
    {
      if (keypadInputPass == keypadPass2)
      {
        lcd.clear();
        lcd.setCursor(2, 1);
        lcd.print("moi: ");
        keypadPass2 = getPassword();
        for (int i = pass1Addr; i<pass2Addr; i++)
        {
            EEPROM.write(i, keypadPass2[i-pass1Addr]);
            delay(5);
        }
        myDelay(500);
        lcd.clear();
        lcd.setCursor(3,0);
        lcd.print("hoan thanh");
        return 1;
       }
       else
       {
         keypadInputPass = "";
         lcd.setCursor(2, 0);
         lcd.print("Sai mat khau ");
         return 1;
       }
     }
   }
   else if (keypadData == 'C')
   {
     lcd.clear();
     lcd.setCursor(2, 1);
     lcd.print("C: ");
     keypadInputPass = getPassword();
     if (keypadInputPass.length() == 4)
     {
       if (keypadInputPass == keypadPass3)
       {
         lcd.clear();
         lcd.setCursor(2, 1);
         lcd.print("moi: ");
         keypadPass3 = getPassword();
         for (int i = pass2Addr; i<pass3Addr; i++)
         {
            EEPROM.write(i, keypadPass3[i-pass2Addr]);
            delay(5);
         }
         myDelay(500);
         lcd.clear();
         lcd.setCursor(3,0);
         lcd.print("hoan thanh");
         return 1;
       }
      else
      {
        keypadInputPass = "";
        lcd.setCursor(2, 0);
        lcd.print("Sai mat khau ");
        return 1;
       }
     }
   }
   else if (keypadData == 'D')
   {
     lcd.clear();
     lcd.setCursor(2, 1);
     lcd.print("D: ");
     keypadInputPass = getPassword();
     if (keypadInputPass.length() == 4)
     {
       if (keypadInputPass == keypadPass4)
       {
         lcd.clear();
         lcd.setCursor(2, 1);
         lcd.print("moi: ");
         keypadPass4 = getPassword();
         for (int i = pass3Addr; i<pass4Addr; i++)
         {
            EEPROM.write(i, keypadPass4[i-pass3Addr]);
            delay(5);
         }
         myDelay(500);
         lcd.clear();
         lcd.setCursor(3,0);
         lcd.print("hoan thanh");
         return 1;
       }
      else
      {
        keypadInputPass = "";
        lcd.setCursor(2, 0);
        lcd.print("Sai mat khau ");
        return 1;
       }
     }
    }
   return 0;
}

// hảm đọc thẻ và kiểm tra thẻ
int checkCard()
{
  int count1 = 0, count2 = 0;
  // tìm thẻ
  if (mfrc522.PICC_IsNewCardPresent())
  {
    // đọc thẻ
    if (mfrc522.PICC_ReadCardSerial())
    {
      // hảm kiểm tra với thẻ đã lưu 
      for (int i = 0; i < mfrc522.uid.size; i++)
      {
        uidDec[i] = mfrc522.uid.uidByte[i];
        if (uidDec[i] ==  cardID1[i])
          count1++;
        if (uidDec[i] ==  cardID2[i])
          count2++;
        if (count1 == 4 || count2 == 4)
        {
          uidDec[0] = 0;
          uidDec[1] = 0;
          uidDec[2] = 0;
          uidDec[3] = 0;
          return true;
        }
      }
    }
  }
  return false;
}

// hàm nhận và giải mã chuỗi json
void parseJson()
{
  // khai báo chuỗi để lưu
  String stringReceiveFromSlave = "";
  unsigned long timeReadSerial;

  if (mySerial.available() > 0)
  {
    // kiểm tra thời gian, nếu quá 1s sẽ thoát
    timeReadSerial = millis();
    while ((millis() - timeReadSerial < TIME_OUT_READ_SERIAL)) {
      if (mySerial.available() > 0) {
        // lưu vào chuỗi
        char cmd = mySerial.read();
        stringReceiveFromSlave += cmd;
        // nhận được ký tự cuối cùng thì bắt đầu giải mã
        if (cmd == '}') {
          StaticJsonBuffer<200> jsonBuffer;
          JsonObject& root = jsonBuffer.parseObject(stringReceiveFromSlave);
          // lưu giá trị vào biến COMMAND và ID
          COMMAND = root["COMMAND"];
          ID = root["ID"];
          break;
        }
      }
    }
  }
}

// hàm gửi trạng thái
void sendStatus(char *gateStatus, char *lightStatus, char *doorStatus)
{
  // cấu trúc gửi {CC.CUA: 0, CC.DEN: 1, CN.CUA: 1}
  // khai báo biến json
  StaticJsonBuffer<200> jsonBuffer;
  // tạo chuỗi json
  JsonObject& root = jsonBuffer.createObject();
  // gán các giá trị vào chuỗi
  root["CC.CUA"] = gateStatus;
  root["CC.DEN"] = lightStatus;
  root["CN.CUA"] = doorStatus;
  // gửi chuỗi
  root.printTo(mySerial);
}

// hàm mở cổng
void openGate()
{
  SVgate.write(1);
  gateStatus = "1";
}

// hàm đóng cổng
void closeGate()
{
  SVgate.write(179);
  gateStatus = "0";
}


// hàm mở cửa
void openDoor()
{
  SVdoor.write(1);
  doorStatus = "1";
  lcd.setCursor(3, 0);
  lcd.print("Cua mo");
}

// hàm đóng cửa
void closeDoor()
{
  SVdoor.write(179);
  doorStatus = "0";
  lcd.setCursor(3, 0);
  lcd.print("Cua da dong");
}

// hàm bật đèn
void lightOn()
{
  digitalWrite(light, LOW);
  lightStatus = "1";
}

// hàm tắt đèn
void lightOff()
{
  digitalWrite(light, HIGH);
  lightStatus = "0";
}

// Hàm delay 
void myDelay(unsigned long delayTime)
{
  unsigned long functionTime = millis();
  while (( millis() - functionTime) < delayTime)
  {}
}
