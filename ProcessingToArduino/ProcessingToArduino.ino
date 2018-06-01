char val;
int ledPin = 13;
boolean ledState = LOW;


void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
  establishContact();
}

void loop() {
  if(Serial.available()){
    val = Serial.read();
    if(val == '1')
    {
    ledState = !ledState;
    digitalWrite(ledPin, ledState);
    }
    delay(100);
  }
  else{
     Serial.println("Hello, world");
     digitalWrite(ledPin, LOW);
     delay(50);
    }
}

void establishContact(){
  while (Serial.available() <= 0){
  Serial.println("A");
  delay(300);
  }
}

