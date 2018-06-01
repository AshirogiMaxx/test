int phase1 = 2;
int phase2 = 3;
int phase3 = 4;
int led = 13;

unsigned long stepLength = 10000;
int minStepLength = 1400;
int steps = 5;

void setup() {
  pinMode(led, OUTPUT);  
  pinMode(phase1, OUTPUT);
  pinMode(phase2, OUTPUT);
  pinMode(phase3, OUTPUT);
  digitalWrite(led, LOW); 
}

void loop() {
  switchStep(1);
  switchStep(2); 
  switchStep(3);
  stepLength--;
}

void switchStep(int stage)
{
  switch(stage)
  {
  case 1:
    digitalWrite(phase1, HIGH);
    digitalWrite(phase2, LOW);
    digitalWrite(phase3, LOW);
    myDelay(stepLength);
    break;
    
  case 2:
    digitalWrite(phase1, LOW);
    digitalWrite(phase2, HIGH);
    digitalWrite(phase3, LOW);
    myDelay(stepLength);
    break;
    
  default:
    digitalWrite(phase1, LOW);
    digitalWrite(phase2, LOW);
    digitalWrite(phase3, HIGH);
    myDelay(stepLength);
    break;
  }  


}

void myDelay(unsigned long p) {
  delayMicroseconds(p);
  
}


