//#include <stdio.h>
//#include <string.h>

int hola;
struct {
      unsigned int uno;
      unsigned int dos;  
}numero1;

struct {
      unsigned int uno : 1;
      unsigned int dos : 1;  
}numero2;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
   Serial.println(sizeof(numero1));
   Serial.println(sizeof(numero2));
   Serial.println(sizeof(hola));
   Serial.println('\n');


}
