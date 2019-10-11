#include <ArduinoJson.h>
#include<SoftwareSerial.h>
#include<Servo.h>
#include "DHT.h"
#include "HC595.h"


#define TX_Pin  5   //5
#define RX_Pin  6   //6

#define DHT_Pin       13
#define Gas_Sensor    A0
#define Cua_N_Bep     11  //Servo
#define Rain_Sensor   A1

#define Time_Out  15000
SoftwareSerial ss(RX_Pin,TX_Pin);
Servo myservo;

const int DHT_type = DHT11;  //Khai báo loại cảm biến, có 2 loại là DHT11 và DHT22
DHT dht(DHT_Pin, DHT_type);

// using HC595 to control device with pin 7 8 9 10

// cua keo cho phoi do 3pins , cua keo gara 3 pins
#define  ENA          2
#define  IN1          A4
#define  IN2          A5

#define  IN3          A2
#define  IN4          A3
#define  ENB          4
#define  STBY         3

#define  Time_Delay_Gara   1000  //ms
#define  Time_Delay_Door   1000  //ms
#define  f_gara     30          //ms
#define  f_door     150          //ms
#define  temp_gara  Time_Delay_Gara/f_gara
#define  temp_door  Time_Delay_Door/f_door


// value for servo
const int Open = 170;
const int Close = 10;

int Rain=0;
int Gas=0;
float Humi=0;
float T=0;
String Data_Control="";

/* Status Devices**/
#define TEMP      0
#define HUMI      1
#define GAS       2
#define RAIN      3
#define NB_CUASO  4
#define NB_DEN    5       //1
#define NB_QUAT   6       //2
#define NK_QUAT   7       //3
#define NK_DEN    8       //4
#define PN_DEN    9       //5
#define PN_QUAT   10      //6
#define PT_DEN    11      //7
#define PD_CUA    12
#define GR_CUA    13

byte Control=0;
unsigned int statusDevices [14] = {};
void INIT(){
  Serial.begin(9600);
  ss.begin(9600);
  myservo.attach(Cua_N_Bep);
  myservo.write(Close);
  pinMode(Gas_Sensor,INPUT);
  pinMode(Rain_Sensor,INPUT);
  HC595_INIT();
  pinMode(STBY,OUTPUT);
  digitalWrite(STBY,HIGH);
  
}

void Control_Device(byte Control_Signal){
  
  Shift_Data(Control_Signal);
  //Serial.println(Control_Signal,BIN);
  digitalWrite(STCP,1);
  delay(10);
  digitalWrite(STCP,0);
}

void Turn_On(byte Device){
  byte temp=1<<(Device-5);  
  Control |= temp; 
  Control_Device(Control);
}
void Turn_Off(byte Device){
  byte temp=1<<(Device-5);
  temp =~temp;
  Control &= temp; 
  Control_Device(Control);
}
void Keo_Cua_N_Bep(int data){
  if(data==Open){
    statusDevices[NB_CUASO]=1;
  }else{
    statusDevices[NB_CUASO]=0;
  }
  myservo.write(data);        // open or close
}

void Check_Stt_Devices(){
  statusDevices[NB_DEN]=bitRead(Control,NB_DEN-5);
  statusDevices[NB_QUAT]=bitRead(Control,NB_QUAT-5);
  statusDevices[NK_QUAT]=bitRead(Control,NK_QUAT-5);
  statusDevices[NK_DEN]=bitRead(Control,NK_DEN-5);
  statusDevices[PN_DEN]=bitRead(Control,PN_DEN-5);
  statusDevices[PN_QUAT]=bitRead(Control,PN_QUAT-5);
  statusDevices[PT_DEN]=bitRead(Control,PT_DEN-5);
  
  String Send="{\"TEMP\":" + String(T) + ",\"HUMI\":" + String(Humi)+ ",\"GAS\":"+String(Gas)+",\"RAIN\":"+String(Rain)+ ",\"NB.CUASO\":"+
    String(statusDevices[NB_CUASO])+ ",\"NB.DEN\":" +String(statusDevices[NB_DEN])+",\"NB.QUAT\":"+String(statusDevices[NB_QUAT])+
    ",\"NK.QUAT\":"+String(statusDevices[NK_QUAT]) + ",\"NK.DEN\":" + String(statusDevices[NK_DEN]) + ",\"PN.DEN\":" +String(statusDevices[PN_DEN]) +
    ",\"PN.QUAT\":"+String(statusDevices[PN_QUAT]) + ",\"PT.DEN\":" + String(statusDevices[PT_DEN]) + ",\"PD.CUA\":"+ String(statusDevices[PD_CUA]) + 
    ",\"GR.CUA\":" + String(statusDevices[GR_CUA]) + "}";
  //String Send="{\"TEMP\":100,\"HUMI\":50,\"GAS\":59,\"RAIN\":47,\"NB.CUASO\":0,\"NB.DEN\":1,\"NB.QUAT\":0,\"NK.QUAT\":1,\"NK.DEN\":0,\"PN.DEN\":0,\"PN.QUAT\":0,\"PT.DEN\":1,\"PD.CUA\":0,\"GR.CUA\":1}";
  Serial.println(Send);
  ss.print(Send);
}

void Keo_Cua_Gara(boolean Status){
  statusDevices[GR_CUA]=Status;
  Check_Stt_Devices();  
  digitalWrite(IN2,Status);
  digitalWrite(IN1,!Status);
  for(int i=0; i<4;i++){
    digitalWrite(ENA,1);
    digitalWrite(STBY,HIGH);
    delay(f_gara);
    digitalWrite(ENA,0);
    digitalWrite(STBY,HIGH);
    delay(f_gara);
  }

}

void Keo_Cua_PD(boolean Status){
  statusDevices[PD_CUA]=Status;
  Check_Stt_Devices();
  digitalWrite(IN3,Status);
  digitalWrite(IN4,!Status);
  for(int i=0; i<temp_door; i++){
    digitalWrite(ENB,1);
    digitalWrite(STBY,HIGH);
    delay(f_door);
    digitalWrite(ENB,0);
    digitalWrite(STBY,HIGH);
    delay(f_door);
  }

}
