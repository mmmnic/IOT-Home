#include"function.h"

// Test Rececive: {"TEMP":0,"HUMI":0,"GAS":0,"RAIN":0,"COMMAND":1,"ID":"NB","Status":"String"}
// Send Demo {"TEMP":100,"HUMI":50,"GAS":59,"RAIN":47,"NB.CUASO":0,"NB.DEN":1,"NB.QUAT":0,"NK.QUAT":1,"NK.DEN":0,"PN.DEN":0,"PN.QUAT":0,"PT.DEN":1,"PD.CUA":0,"GR.CUA":1}

unsigned long Time;
void setup() {
  INIT();
  Time=millis();

}

void loop() {
  // Read Temp and Humi every 5'
  if(millis()-Time>Time_Out){
    Gas = analogRead(Gas_Sensor);
    Rain = analogRead(Rain_Sensor);
    Humi = dht.readHumidity();    
    T = dht.readTemperature();  
    Time=millis();  
  }

  /* Receive Uart ******************************/
  if(ss.available()>0){
    char character = char(ss.read());
    Serial.write(character);
    Data_Control += character;
    if(character == '}'){
      Serial.println(Data_Control);
      DynamicJsonBuffer jsonBuffer;
      JsonObject &MyObject = jsonBuffer.parseObject(Data_Control);
      if (!MyObject.success()) {
        Serial.println("parseArray() failed");
      } 
      //Serial.println("parseArray() success");
      String ID = MyObject["ID"];
      int temp = MyObject["COMMAND"];
      // start control
      if(temp == 99){
        /* send update status*/
        Check_Stt_Devices();
      }
      
      if(ID=="NB" ){
        
        switch (temp) {
          case 1: Keo_Cua_N_Bep (Open);   break;
          case 2: Keo_Cua_N_Bep (Close);  break;
          case 3: Turn_On (NB_DEN);       break;
          case 4: Turn_Off (NB_DEN);      break;
          case 5: Turn_On (NB_QUAT);      break;
          case 6: Turn_Off (NB_QUAT);     break;
          
          default: break;
        }
        Check_Stt_Devices();
      } 

      if(ID=="NK" ){
        switch (temp) {
          case 3: Turn_On (NK_DEN);    break;
          case 4: Turn_Off (NK_DEN);   break;
          case 5: Turn_On (NK_QUAT);   break;
          case 6: Turn_Off (NK_QUAT);  break;
          
          default: break;
        }
        Check_Stt_Devices();
      }
      
      if(ID=="PN" ){
        switch (temp) {
          case 3: Turn_On (PN_DEN);    break;
          case 4: Turn_Off (PN_DEN);   break;
          case 5: Turn_On (PN_QUAT);   break;
          case 6: Turn_Off (PN_QUAT);  break;
          
          default: break;
        }
        Check_Stt_Devices();
      }

      if(ID=="PT" ){
        switch (temp) {
          case 3: Turn_On (PT_DEN);    break;
          case 4: Turn_Off (PT_DEN);   break;
   
          default: break;
        }
        Check_Stt_Devices();
      }

      if(ID=="PD" ){
        switch (temp) {
          case 1: Keo_Cua_PD (1);         break;
          case 2: Keo_Cua_PD (0);         break;
          
          default: break;
        }
      } 

      if(ID=="GR" ){
        switch (temp) {
          case 1: Keo_Cua_Gara (1);         break;
          case 2: Keo_Cua_Gara (0);         break;
          
          default: break;
        }
        
      } 
      
      Data_Control=""; // reset datacontrol                 
    } 
    
    
  }
  
  
  
}
