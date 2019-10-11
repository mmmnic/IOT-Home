// FUNCTION FOR HC 595

#define SHCP  8
#define DS    10
#define STCP  9
#define RS    7

void HC595_INIT(){
  pinMode(SHCP,OUTPUT);
  pinMode(STCP,OUTPUT);
  pinMode(DS,OUTPUT);
  pinMode(RS,OUTPUT);
}


void Reset_HC595(){
  digitalWrite(RS,0);
  digitalWrite(RS,1);
}

void Shift_Bit(byte data){
  if(data==1){
    digitalWrite(DS,1);
  }
  else{
    digitalWrite(DS,0);
  }
  digitalWrite(SHCP,1);
  digitalWrite(SHCP,0);
}

void Shift_Data(byte data){
  Reset_HC595();
  digitalWrite(STCP,1);
  delay(10);
  digitalWrite(STCP,0);
  for(int i=0; i<8; i++){
    Shift_Bit(bitRead(data,i));
  }
}
