/*use json type: {"TEMP":  ,"HUMI":   ,"GAS":  ,"RAIN":   ,"COMMAND": int ,"ID": string  ,"STATUS": int}*/


#include <ArduinoJson.h>

#include <ESP8266WiFi.h>

#include <SocketIoClient.h>

SocketIoClient webSocket;

const char* Host_Socket = "192.168.137.1";
unsigned int Port_Socket = 3000;
const char* ssid = "lee-lap";
const char* pwdWifi = "lee221221";
//==========================================================================================
//                                    Data current
//==========================================================================================
#define ON 1
#define OFF 0
uint8_t LED2_Status=OFF;
int A0_Val=0;

//==========================================================================================
//                                    Interval time
//==========================================================================================

unsigned long intervalTimeSend;
#define TIME_SEND 5000

//==========================================================================================
//                                    Socket IO
//==========================================================================================

void handleMessControlDevice(const char * payload, size_t length) {
  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(payload);
  String ID = root["ID"];
  uint8_t COMMAND = root["COMMAND"];
  if (ID == "LED2") { //led on board
    if(COMMAND == 0) { //tat den
      digitalWrite(2, HIGH);
      LED2_Status = 0;
      sendServer();
    }
    if(COMMAND == 1) { //bat den
      digitalWrite(2, LOW);
      LED2_Status = 1;
      sendServer();
    }
  }
}

void handleMessUpdate(const char * payload, size_t length) {
  sendServer();
}

void(* resetFunc) (void) = 0;// reset function

void sendServer () {
      A0_Val = random(1000);
      char sendStr[300]={};
      String output = "[{\"TYPE\":\"LOG\",\"ID\":\"A0\",\"STATUS\":"+String(A0_Val)+"},{\"TYPE\":\"O/F\",\"ID\":\"LED2\",\"STATUS\":"+String(LED2_Status)+"}]";
      int i=0;
      for (i=0; output[i]; i++) {
        sendStr[i]=output[i];
      }
      sendStr[i]='\0';
      webSocket.emit("esp-send-server-update-status", sendStr);
}

//==========================================================================================
//                                    Setup
//==========================================================================================

void setup() {
    Serial.begin(115200);
    Serial.setDebugOutput(true);

    randomSeed(analogRead(0));
    pinMode (2, OUTPUT);
    digitalWrite(2, HIGH);
    LED2_Status=OFF;

    A0_Val = random(1000);

    Serial.println();
    Serial.println();
    Serial.println();

    Serial.println("aa");

    WiFi.begin(ssid, pwdWifi);

    Serial.println("aa");

    while (WiFi.status() != WL_CONNECTED)
    {
      delay(500);
      Serial.print(".");
    }
  
    Serial.println();
    Serial.print("Connected, IP address: ");
    Serial.println(WiFi.localIP());
    
    webSocket.on("server-send-esp-control-device", handleMessControlDevice);
    webSocket.on("server-send-esp-update-status", handleMessUpdate);
    
    webSocket.begin(Host_Socket, Port_Socket, "/socket.io/?transport=websocket");
    
    delay(1000);
    intervalTimeSend=millis();
}

//==========================================================================================
//                                    Loop
//==========================================================================================

void loop() {
    if(Serial.available()>0) {
      char cmd = Serial.read();
      if (cmd == '0') {
        LED2_Status=OFF;
        digitalWrite(2, HIGH);
      }
      if (cmd == '1') {
        LED2_Status=ON;
        digitalWrite(2, LOW);
      }
      if (cmd == '0' || cmd == '1') {
        sendServer();
      }
    }
    if (millis() - intervalTimeSend > TIME_SEND) {
      intervalTimeSend=millis();
      sendServer();
    }
    webSocket.loop();
}
