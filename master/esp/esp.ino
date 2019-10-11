/*use json type: {"TEMP":  ,"HUMI":   ,"GAS":  ,"RAIN":   ,"COMMAND": int ,"ID": string  ,"STATUS": int}*/


#include <ArduinoJson.h>

#include <ESP8266WiFi.h>

#include <SocketIoClient.h>

#include <SoftwareSerial.h>

SocketIoClient webSocket;

const char* Host_Socket = "192.168.137.1";
unsigned int Port_Socket = 8000;
const char* ssid = "lee-lap";
const char* pwdWifi = "lee221221";

//==========================================================================================
//                                    Interval time
//==========================================================================================

unsigned long intervalTimeUpdateDevices;
unsigned long intervalTimeUpdateDoors;
unsigned long intervalTimeUpdateNano;
#define TIME_UPDATE_DEVICES 5000
#define TIME_UPDATE_DOORS 5000
#define TIME_UPDATE_NANO 5000

unsigned long timeReadSerial;
#define TIME_OUT_READ_SERIAL 1000

unsigned long intervalTimeSend;
#define TIME_SEND_SOCKET 5000
//==========================================================================================
//                                    Data current
//==========================================================================================
#define ON 1
#define OFF 0

#define TEMP      0
#define HUMI      1
#define GAS       2
#define RAIN      3
#define NB_CUASO  4
#define NB_DEN    5
#define NB_QUAT   6
#define NK_QUAT   7
#define NK_DEN    8
#define PN_DEN    9
#define PN_QUAT   10
#define PT_DEN    11
#define PD_CUA    12
#define GR_CUA    13
#define CC_CUA    14
#define CC_DEN    15
#define CN_CUA    16


#define MO_CUA 1
#define DONG_CUA 2
#define MO_DEN 3
#define TAT_DEN 4
#define MO_QUAT 5
#define TAT_QUAT 6
unsigned int statusDevices [17] = {};
String statusNB="";
String statusNK="";
String statusPN="";
String statusPT="";
String statusPD="";
String statusGR="";
String statusCC="";
String statusCN="";
//==========================================================================================
//                                    Communicate Slave
//==========================================================================================
#define CMD_JUST_UPDATE 99

String stringSendToSlave = "";

void processStringSendToSlave (String ID, uint8_t COMMAND) {
  stringSendToSlave += "{\"ID\":\""+ID+"\",\"COMMAND\":"+String(COMMAND)+"}";
}

void updateStringStatus() {
  //---------nha bep
  statusNB="";
  if(statusDevices[NB_DEN] == ON) {
    statusNB += "-Light On ";
  } else {
    statusNB += "-Light Off ";
  }
  if(statusDevices[NB_QUAT] == ON) {
    statusNB += "-Fan On ";
  } else {
    statusNB += "-Fan Off ";
  }
  if(statusDevices[NB_CUASO] == ON) {
    statusNB += "-Door Open ";
  } else {
    statusNB += "-Door Close ";
  }
  //--------nha khach
  statusNK="";
  if(statusDevices[NK_DEN] == ON) {
    statusNK += "-Light On ";
  } else {
    statusNK += "-Light Off ";
  }
  if(statusDevices[NK_QUAT] == ON) {
    statusNK += "-Fan On ";
  } else {
    statusNK += "-Fan Off ";
  }
  //---------phong ngu
  statusPN="";
  if(statusDevices[PN_DEN] == ON) {
    statusPN += "-Light On ";
  } else {
    statusPN += "-Light Off ";
  }
  if(statusDevices[PN_QUAT] == ON) {
    statusPN += "-Fan On ";
  } else {
    statusPN += "-Fan Off ";
  }
  //---------phong tam
  statusPT="";
  if(statusDevices[PT_DEN] == ON) {
    statusPT += "-Light On ";
  } else {
    statusPT += "-Light Off ";
  }
  //----------phoi do
  statusPD="";
  if(statusDevices[PD_CUA] == ON) {
    statusPD += "-Door Open ";
  } else {
    statusPD += "-Door Close ";
  }
  //----------gara
  statusGR="";
  if(statusDevices[GR_CUA] == ON) {
    statusGR += "-Door Open ";
  } else {
    statusGR += "-Door Close ";
  }
  //----------cua cong
  statusCC="";
  if(statusDevices[CC_CUA] == ON) {
    statusCC += "-Door Open ";
  } else {
    statusCC += "-Door Close ";
  }
  if(statusDevices[CC_DEN] == ON) {
    statusCC += "-Light On ";
  } else {
    statusCC += "-Light Off ";
  }
  //----------cua nha
  statusCN="";
  if(statusDevices[CN_CUA] == ON) {
    statusCN += "-Door Open ";
  } else {
    statusCN += "-Door Close ";
  }
}

String stringReceiveFromSlave = "";
bool receiveOK = false;
//------------------SLAVE DEVICES-------------------
SoftwareSerial SERIAL_DEVICES(D6,D5); // RX, TX
//#define SERIAL_DEVICES Serial
void processStringReceiveFromSlaveDevices() {
  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(stringReceiveFromSlave);
  
  //analog
  statusDevices[TEMP] = root["TEMP"];
  statusDevices[HUMI] = root["HUMI"];
  statusDevices[GAS] = root["GAS"];
  statusDevices[RAIN] = root["RAIN"];
  //nha bep
  statusDevices[NB_CUASO] = root["NB.CUASO"];
  statusDevices[NB_DEN] = root["NB.DEN"];
  statusDevices[NB_QUAT] = root["NB.QUAT"];
  //nha khach
  statusDevices[NK_QUAT] = root["NK.QUAT"];
  statusDevices[NK_DEN] = root["NK.DEN"];
  //phong ngu
  statusDevices[PN_DEN] = root["PN.DEN"];
  statusDevices[PN_QUAT] = root["PN.QUAT"];
  //phong tam
  statusDevices[PT_DEN] = root["PT.DEN"];
  //phoi do
  statusDevices[PD_CUA] = root["PD.CUA"];
  //GARA
  statusDevices[GR_CUA] = root["GR.CUA"];
}
void sendCMDToDevices(String ID, uint8_t CMD, bool debugTerminal = false) {
    stringSendToSlave = "";
    processStringSendToSlave (ID, CMD);
    SERIAL_DEVICES.print(stringSendToSlave);
    timeReadSerial = millis();
    while ((millis() - timeReadSerial < TIME_OUT_READ_SERIAL) && !receiveOK) {
      if(SERIAL_DEVICES.available()>0) {
        char cmd = SERIAL_DEVICES.read();
        stringReceiveFromSlave+=cmd;
        if(cmd == '}') {
          receiveOK = true;
          break;
        }
      }
    }
    if(debugTerminal) {
      Serial.println(stringReceiveFromSlave);
    }
    if(receiveOK) {
      if(debugTerminal) {
        Serial.println();
        Serial.println("update ok");
        Serial.println(stringReceiveFromSlave);
      }
      processStringReceiveFromSlaveDevices();
      updateStringStatus();
      stringReceiveFromSlave="";
      receiveOK=false;
    } else {
      stringReceiveFromSlave="";
      if(debugTerminal) {
        Serial.println();
        Serial.println("update fail");
      }
    }
    if(debugTerminal) {
      Serial.println("TEMP: "+String(statusDevices[TEMP]));
      Serial.println("HUMI: "+String(statusDevices[HUMI]));
      Serial.println("GAS: "+String(statusDevices[GAS]));
      Serial.println("RAIN: "+String(statusDevices[RAIN]));
      Serial.println("nha bep:");
      Serial.println(statusNB);
      Serial.println("nha khach:");
      Serial.println(statusNK);
      Serial.println("phong tam:");
      Serial.println(statusPT);
      Serial.println("phong ngu:");
      Serial.println(statusPN);
      Serial.println("cho phoi do:");
      Serial.println(statusPD);
      Serial.println("gara:");
      Serial.println(statusGR);
    }
}
//------------------SLAVE DOOR-------------------
SoftwareSerial SERIAL_DOORS(D3,D4); // RX, TX
void processStringReceiveFromSlaveDoors() {
  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(stringReceiveFromSlave);
  //cua cong
  statusDevices[CC_CUA] = root["CC.CUA"];
  statusDevices[CC_DEN] = root["CC.DEN"];
  //cua nha
  statusDevices[CN_CUA] = root["CN.CUA"];
}
void sendCMDToDoors(String ID, uint8_t CMD, bool debugTerminal = false) {
    stringSendToSlave = "";
    processStringSendToSlave (ID, CMD);
    SERIAL_DOORS.print(stringSendToSlave);
    timeReadSerial = millis();
    SERIAL_DEVICES.flush();
    while ((millis() - timeReadSerial < TIME_OUT_READ_SERIAL) && !receiveOK) {
      if(SERIAL_DOORS.available()>0) {
        char cmd = SERIAL_DOORS.read();
        stringReceiveFromSlave+=cmd;
        if(cmd == '}') {
          receiveOK = true;
          break;
        }
      }
    }
    if(receiveOK) {
      if(debugTerminal) {
        Serial.println();
        Serial.println("update ok");
      }
      processStringReceiveFromSlaveDoors();
      updateStringStatus();
      stringReceiveFromSlave="";
      receiveOK=false;
    } else {
      stringReceiveFromSlave="";
      if(debugTerminal) {
        Serial.println();
        Serial.println("update fail");
      }
    }
    if(debugTerminal) {
      Serial.println("cua cong:");
      Serial.println(statusCC);
      Serial.println("cua nha:");
      Serial.println(statusCN);
    }
}
//------------------SLAVE NANO-------------------
SoftwareSerial SERIAL_NANO(D2,D1); // RX, TX
void processStringReceiveFromSlaveNano() {
  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(stringReceiveFromSlave);
  String Mess = root["Mess"];
//  myString.indexOf(val)
//  myString.indexOf(val, from)
  int16_t indexOfChar = Mess.indexOf((char)0x01);
  int16_t startPos = 0;
  while (indexOfChar != -1) {
    String tempStr = "";
    for (;startPos < indexOfChar; startPos++) {
      tempStr += Mess[startPos];
    }

    //handle message sms right here:
    if(tempStr == "TAT DEN NHA BEP") { 
      sendCMDToDevices("NB",TAT_DEN,false);
    }
    if(tempStr == "MO DEN NHA BEP") { 
      sendCMDToDevices("NB",MO_DEN,false);
    }

    if(tempStr == "TAT QUAT NHA BEP") { 
      sendCMDToDevices("NB",TAT_QUAT,false);
    }
    if(tempStr == "MO QUAT NHA BEP") { 
      sendCMDToDevices("NB",MO_QUAT,false);
    }

    if(tempStr == "MO CUA SO NHA BEP") { 
      sendCMDToDevices("NB",MO_CUA,false);
    }
    if(tempStr == "DONG CUA SO NHA BEP") { 
      sendCMDToDevices("NB",DONG_CUA,false);
    }

    if(tempStr == "TAT DEN NHA KHACH") { 
      sendCMDToDevices("NK",TAT_DEN,false);
    }
    if(tempStr == "MO DEN NHA KHACH") { 
      sendCMDToDevices("NK",MO_DEN,false);
    }

    if(tempStr == "TAT QUAT NHA KHACH") { 
      sendCMDToDevices("NK",TAT_QUAT,false);
    }
    if(tempStr == "MO QUAT NHA KHACH") { 
      sendCMDToDevices("NK",MO_QUAT,false);
    }

    if(tempStr == "TAT DEN PHONG NGU") { 
      sendCMDToDevices("PN",TAT_DEN,false);
    }
    if(tempStr == "MO DEN PHONG NGU") { 
      sendCMDToDevices("PN",MO_DEN,false);
    }

    if(tempStr == "TAT QUAT PHONG NGU") { 
      sendCMDToDevices("PN",TAT_QUAT,false);
    }
    if(tempStr == "MO QUAT PHONG NGU") { 
      sendCMDToDevices("PN",MO_QUAT,false);
    }

    if(tempStr == "TAT DEN PHONG TAM") { 
      sendCMDToDevices("PT",TAT_DEN,false);
    }
    if(tempStr == "MO DEN PHONG TAM") { 
      sendCMDToDevices("PT",MO_DEN,false);
    }

    if(tempStr == "MO CUA PHOI DO") { 
      sendCMDToDevices("PD",MO_CUA,false);
    }
    if(tempStr == "DONG CUA PHOI DO") { 
      sendCMDToDevices("PD",DONG_CUA,false);
    }

    if(tempStr == "MO CUA GARA") { 
      sendCMDToDevices("GR",MO_CUA,false);
    }
    if(tempStr == "DONG CUA GARA") { 
      sendCMDToDevices("GR",DONG_CUA,false);
    }

    if(tempStr == "MO CUA CONG") { 
      sendCMDToDevices("CC",MO_CUA,false);
    }
    if(tempStr == "DONG CUA CONG") { 
      sendCMDToDevices("CC",DONG_CUA,false);
    }

    if(tempStr == "MO DEN CUA CONG") { 
      sendCMDToDevices("CC",MO_DEN,false);
    }
    if(tempStr == "TAT DEN CUA CONG") { 
      sendCMDToDevices("CC",TAT_DEN,false);
    }

    if(tempStr == "MO CUA NHA") { 
      sendCMDToDevices("CN",MO_CUA,false);
    }
    if(tempStr == "DONG CUA NHA") { 
      sendCMDToDevices("CN",DONG_CUA,false);
    }
    
    if(indexOfChar == Mess.length() - 1) {//read the end
      break;
    }
    startPos = indexOfChar+1;
    indexOfChar = Mess.indexOf((char)0x01, indexOfChar+1);
  }
  updateStringStatus();
  sendServer();
}
void sendCMDToNano(bool debugTerminal = false) {
    //get mess from sim
    stringSendToSlave = "";
    SERIAL_NANO.write(0x05); //gui ky tu nay de cap nhat lenh tin nhan tu sim
    timeReadSerial = millis();
    SERIAL_NANO.flush();
    while ((millis() - timeReadSerial < TIME_OUT_READ_SERIAL) && !receiveOK) {
      if(SERIAL_NANO.available()>0) {
        char cmd = SERIAL_NANO.read();
        stringReceiveFromSlave+=cmd;
        if(cmd == '}') {
          receiveOK = true;
          break;
        }
      }
    }
    if(receiveOK) {
      if(debugTerminal) {
        Serial.println();
        Serial.println("update ok");
      }
      processStringReceiveFromSlaveNano();
      updateStringStatus();
      stringReceiveFromSlave="";
      receiveOK=false;
    } else {
      stringReceiveFromSlave="";
      if(debugTerminal) {
        Serial.println();
        Serial.println("update fail");
      }
    }
    //send humi, temp, gas, rain to show lcd
    stringSendToSlave = "{\"TEMP\":"+String(statusDevices[TEMP])+",\"HUMI\":"+String(statusDevices[HUMI])+",\"GAS\":"+String(statusDevices[GAS])+",\"RAIN\":"+String(statusDevices[RAIN])+"}";
    SERIAL_NANO.print(stringSendToSlave);
    if(debugTerminal) {
        Serial.println();
        Serial.print("send to nano: ");
        Serial.println(stringSendToSlave);
    }
}
//==========================================================================================
//                                    Socket IO
//==========================================================================================

void handleMessControlDevice(const char * payload, size_t length) {
  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(payload);
  String ID = root["IDNODE"];
  uint8_t COMMAND = root["COMMAND"];
  if(ID == "NB" || ID == "NK" || ID == "PN" || ID == "PT" || ID == "PD" || ID == "GR" || ID == "CC" || ID == "CN") {
    sendCMDToDevices(ID,COMMAND,false);
  }
  if(ID == "CC" || ID == "CN") {
    sendCMDToDoors(ID,COMMAND,false);
  }
  updateStringStatus();
  sendServer();
}

void handleMessUpdate(const char * payload, size_t length) {
  sendServer();
}

void(* resetFunc) (void) = 0;// reset function

char sendStr[300]={};
String output = "";
void sendServer () {
      //nha bep
      output = "{\"TEMP\":"+String(statusDevices[TEMP])+",\"HUMI\":"+String(statusDevices[HUMI])+",\"GAS\":"+String(statusDevices[GAS])+",\"RAIN\":"+String(statusDevices[RAIN])+",\"ID\":\"NB\",\"STATUS\":\""+statusNB+"\"}";
      int i=0;
      for (i=0; output[i]; i++) {
        sendStr[i]=output[i];
      }
      sendStr[i]='\0';
      webSocket.emit("chat-message", sendStr);
      //nha khach
      output = "{\"TEMP\":"+String(statusDevices[TEMP])+",\"HUMI\":"+String(statusDevices[HUMI])+",\"GAS\":"+String(statusDevices[GAS])+",\"RAIN\":"+String(statusDevices[RAIN])+",\"ID\":\"NK\",\"STATUS\":\""+statusNK+"\"}";
      for (i=0; output[i]; i++) {
        sendStr[i]=output[i];
      }
      sendStr[i]='\0';
      webSocket.emit("chat-message", sendStr);
      //phong ngu
      output = "{\"TEMP\":"+String(statusDevices[TEMP])+",\"HUMI\":"+String(statusDevices[HUMI])+",\"GAS\":"+String(statusDevices[GAS])+",\"RAIN\":"+String(statusDevices[RAIN])+",\"ID\":\"PN\",\"STATUS\":\""+statusPN+"\"}";
      for (i=0; output[i]; i++) {
        sendStr[i]=output[i];
      }
      sendStr[i]='\0';
      webSocket.emit("chat-message", sendStr);
      //phong tam
      output = "{\"TEMP\":"+String(statusDevices[TEMP])+",\"HUMI\":"+String(statusDevices[HUMI])+",\"GAS\":"+String(statusDevices[GAS])+",\"RAIN\":"+String(statusDevices[RAIN])+",\"ID\":\"PT\",\"STATUS\":\""+statusPT+"\"}";
      for (i=0; output[i]; i++) {
        sendStr[i]=output[i];
      }
      sendStr[i]='\0';
      webSocket.emit("chat-message", sendStr);
      //phoi do
      output = "{\"TEMP\":"+String(statusDevices[TEMP])+",\"HUMI\":"+String(statusDevices[HUMI])+",\"GAS\":"+String(statusDevices[GAS])+",\"RAIN\":"+String(statusDevices[RAIN])+",\"ID\":\"PD\",\"STATUS\":\""+statusPD+"\"}";
      for (i=0; output[i]; i++) {
        sendStr[i]=output[i];
      }
      sendStr[i]='\0';
      webSocket.emit("chat-message", sendStr);
      //gara
      output = "{\"TEMP\":"+String(statusDevices[TEMP])+",\"HUMI\":"+String(statusDevices[HUMI])+",\"GAS\":"+String(statusDevices[GAS])+",\"RAIN\":"+String(statusDevices[RAIN])+",\"ID\":\"GR\",\"STATUS\":\""+statusGR+"\"}";
      for (i=0; output[i]; i++) {
        sendStr[i]=output[i];
      }
      sendStr[i]='\0';
      webSocket.emit("chat-message", sendStr);
      //cua cong
      output = "{\"TEMP\":"+String(statusDevices[TEMP])+",\"HUMI\":"+String(statusDevices[HUMI])+",\"GAS\":"+String(statusDevices[GAS])+",\"RAIN\":"+String(statusDevices[RAIN])+",\"ID\":\"CC\",\"STATUS\":\""+statusCC+"\"}";
      for (i=0; output[i]; i++) {
        sendStr[i]=output[i];
      }
      sendStr[i]='\0';
      webSocket.emit("chat-message", sendStr);
      //cua nha
      output = "{\"TEMP\":"+String(statusDevices[TEMP])+",\"HUMI\":"+String(statusDevices[HUMI])+",\"GAS\":"+String(statusDevices[GAS])+",\"RAIN\":"+String(statusDevices[RAIN])+",\"ID\":\"CN\",\"STATUS\":\""+statusCN+"\"}";
      for (i=0; output[i]; i++) {
        sendStr[i]=output[i];
      }
      sendStr[i]='\0';
      webSocket.emit("chat-message", sendStr);
}

//==========================================================================================
//                                    Setup
//==========================================================================================

void setup() {
    Serial.begin(115200);
    WiFi.softAPdisconnect (true);
    SERIAL_DEVICES.begin(9600);
    SERIAL_DOORS.begin(9600);
    SERIAL_NANO.begin(9600);
    Serial.setDebugOutput(true);

    Serial.println();
    Serial.println();
    Serial.println();

    WiFi.begin(ssid, pwdWifi);

    while (WiFi.status() != WL_CONNECTED)
    {
      delay(500);
      Serial.print(".");
    }
  
    Serial.println();
    Serial.print("Connected, IP address: ");
    Serial.println(WiFi.localIP());
    
    webSocket.on("chat-message", handleMessControlDevice);
    webSocket.on("update-all", handleMessUpdate);
    
    webSocket.begin(Host_Socket, Port_Socket, "/socket.io/?transport=websocket");
    
    delay(500);
    
//    Serial.println("=======================update devices===============================");
//    sendCMDToDevices("ALL",CMD_JUST_UPDATE,true);
//    Serial.println("=======================update doors===============================");
//    sendCMDToDoors("ALL",CMD_JUST_UPDATE,false);
    intervalTimeUpdateDevices=millis();
    intervalTimeUpdateDoors=millis();
    intervalTimeSend=millis();
}

//==========================================================================================
//                                    Loop
//==========================================================================================
void loop() {
    updateStringStatus();
//    if(millis() - intervalTimeUpdateDevices > TIME_UPDATE_DEVICES) {
//      Serial.println("=======================update devices===============================");
//      intervalTimeUpdateDevices=millis();
//      sendCMDToDevices("ALL",CMD_JUST_UPDATE,false);
//    }
//    if(millis() - intervalTimeUpdateDoors > TIME_UPDATE_DOORS) {
//      Serial.println("=======================update doors===============================");
//      intervalTimeUpdateDoors=millis();
//      sendCMDToDoors("ALL",CMD_JUST_UPDATE,false);
//    }
//    if(millis() - intervalTimeUpdateNano > TIME_UPDATE_NANO) {
//      Serial.println("=======================update nano===============================");
//      intervalTimeUpdateNano=millis();
//      sendCMDToNano(false);
//    }
//    if(millis() - intervalTimeSend > TIME_SEND_SOCKET) {
//      intervalTimeSend=millis();
//      sendServer();
//    }
    webSocket.loop();
}
