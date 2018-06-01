byte ledPin = 13;
byte motionPin = 2;
byte senseMotion = 0;

void setup(){
  pinMode(ledPin, OUTPUT);
  pinMode(motionPin, INPUT);
  Serial.begin(9600);
  while (!Serial);
  Serial.println("Serial inicializado");
}

void loop(){
  senseMotion = digitalRead(motionPin);
    if (senseMotion == HIGH) { 
      digitalWrite(ledPin, HIGH);
      Serial.println("Motion detected");
    } else {                    
      digitalWrite(ledPin, LOW);
    }
}
