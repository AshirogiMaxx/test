//defining pins for eeprom   
int CLOCK =52;          
int DATA_OUT =50;
int DATA_IN =51;
int CHIP_SEL =53;

void setup(){
  pinMode(CLOCK ,OUTPUT);
  pinMode(DATA_OUT ,OUTPUT);
  pinMode(DATA_IN ,INPUT);
  pinMode(CHIP_SEL ,OUTPUT);
  digitalWrite(CHIP_SEL ,LOW);
  Serial.begin(9600);
}
void sendSer(String s){
  digitalWrite(CHIP_SEL,HIGH);
  int len = s.length();
  
  for (int i=0;i<len;i++){
    char c = s.charAt(i);
    if (c=='0')
      digitalWrite(DATA_OUT, LOW);
    else
      digitalWrite(DATA_OUT, HIGH);
    digitalWrite(CLOCK,HIGH); 
    digitalWrite(CLOCK,LOW); 
  }
    //needs little delay to work
    delay(10);
}
void recvSer(String s){
  digitalWrite(CHIP_SEL,HIGH);
  int len = s.length();
  for (int i=0;i<len;i++){
    char c = digitalRead(DATA_IN);
    Serial.print(c==LOW ? "0" : "1");
    digitalWrite(CLOCK,HIGH); 
    digitalWrite(CLOCK,LOW); 
  }
}

void loop(){

  digitalWrite(CHIP_SEL, HIGH);
  sendSer("00000100"); //sending EWEN instruction 
  sendSer("1100000"); //
  digitalWrite(CHIP_SEL,LOW);
  
  digitalWrite(CHIP_SEL, HIGH);
  sendSer("00000101"); //sending WRITE instruction 
  sendSer("0000001"); //sending Address
  sendSer("10001101"); //sending data 
  digitalWrite(CHIP_SEL,LOW);
  
  digitalWrite(CHIP_SEL, HIGH);
  sendSer("00000110"); //sending READ instruction 
  sendSer("0000001"); //sending Address 
  
  //dummy zero that is sent at start of read
  digitalWrite(CLOCK,HIGH);
  digitalWrite(CLOCK,LOW);
  
  //receiving data... zeros is just to specify count.
  //no meaning in them... output is to serial
  recvSer("00001111");
  digitalWrite(CHIP_SEL,LOW);
  
  while(1);
}

