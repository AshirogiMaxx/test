#include <MFRC522.h>
#include <SPI.h>

#define RST_PIN 5
#define SS_PIN 10
MFRC522 mfrc522(SS_PIN, RST_PIN);
const int Relay_Pin = 2;

void setup() {
  pinMode(Relay_Pin, OUTPUT);
  digitalWrite(Relay_Pin, HIGH);
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  Serial.println("Access Control");
  Serial.println("Please move the card next to the surface");
}
byte ActualUID[4];
byte User1[4] = {0x20, 0x4e, 0xbb, 0x4f};
byte User2[4] = {0xc1, 0x2f, 0xd6, 0x0e};

void loop() {

  
  if(mfrc522.PICC_IsNewCardPresent())
  {
      if(mfrc522.PICC_ReadCardSerial())
      {

        Serial.print("Card UID:");
        for(byte i = 0;i < mfrc522.uid.size; i++){
            Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? "0" : " ");
            Serial.print(mfrc522.uid.uidByte[i], HEX);     
            ActualUID[i]=mfrc522.uid.uidByte[i];     
          }
         Serial.println("         ");
         if(compareArray(ActualUID,User1)){
            Serial.println("Acceso concedido");
            digitalWrite(Relay_Pin, LOW);}
         else if(compareArray(ActualUID,User2)){
            Serial.println("Acceso concedido...");
            digitalWrite(Relay_Pin, LOW);}
         else{
            Serial.println("Acceso Denegado....");
            digitalWrite(Relay_Pin, HIGH);}
         mfrc522.PICC_HaltA();
        }
    }
}

 boolean compareArray(byte array1[],byte array2[])
{
  if(array1[0] != array2[0])return(false);
  if(array1[1] != array2[1])return(false);
  if(array1[2] != array2[2])return(false);
  if(array1[3] != array2[3])return(false);
  return(true);
}

