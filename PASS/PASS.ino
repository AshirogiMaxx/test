
void setup() {
pinMode(13, OUTPUT);
  Serial.begin(9600);
  Serial.println("HELLO WORLD!!!!!!!!!!!!!!!!!");
  delay(1000);
  Serial.println("I am Arduino!");
  delay(1000);
    Serial.println("Input the correct passcode and I will tell you the secret!");
}

void loop()  {
  start:
const char passcode = 'lolification';

    if (Serial.read() == passcode) {
        Serial.println("I want Pizza"); //input secret line one here here
        delay(1000);
        Serial.println("I am Cold");//just copy paste these with the delays to keep on adding lines
                delay(1000);
        Serial.println("He broke an office computer!"); 
        delay(1000);
        Serial.println("I like when people program me!!!!!"); 
        delay(1000);
        goto start;
    }
  }
