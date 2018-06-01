#include <Wire.h>

#define MPU 0x68
#define A_R 16384.0
#define G_R 131.0
#define RAD_A_DEG = 57.295779

int16_t AcX, AcY, AcZ, GyX, GyY, GyZ;
int16_t TemP;


float Acc[2];
float Gy[2];
float Angle[2];
float Temp[2];

void setup() {
  // put your setup code here, to run once:

  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
   //Leer los valores del Acelerometro de la IMU
   Wire.beginTransmission(MPU);
   Wire.write(0x3B); //Pedir el registro 0x3B - corresponde al AcX
   Wire.endTransmission(false);
   Wire.requestFrom(MPU,6,true); //A partir del 0x3B, se piden 6 registros
   AcX=Wire.read()<<8|Wire.read(); //Cada valor ocupa 2 registros
   AcY=Wire.read()<<8|Wire.read();
   AcZ=Wire.read()<<8|Wire.read();
 
    //Se calculan los angulos Y, X respectivamente.
   Acc[1] = atan(-1*(AcX/A_R)/sqrt(pow((AcY/A_R),2) + pow((AcZ/A_R),2)))*RAD_TO_DEG;
   Acc[0] = atan((AcY/A_R)/sqrt(pow((AcX/A_R),2) + pow((AcZ/A_R),2)))*RAD_TO_DEG;
 
   //Leer los valores del Giroscopio
   Wire.beginTransmission(MPU);
   Wire.write(0x43);
   Wire.endTransmission(false);
   Wire.requestFrom(MPU,4,true); //A diferencia del Acelerometro, solo se piden 4 registros
   GyX=Wire.read()<<8|Wire.read();
   GyY=Wire.read()<<8|Wire.read();
 
   //Calculo del angulo del Giroscopio
   Gy[0] = GyX/G_R;
   Gy[1] = GyY/G_R;
 
   //Aplicar el Filtro Complementario
   Angle[0] = 0.98 *(Angle[0]+Gy[0]*0.010) + 0.02*Acc[0];
   Angle[1] = 0.98 *(Angle[1]+Gy[1]*0.010) + 0.02*Acc[1];

    Wire.beginTransmission(MPU);
    Wire.write(0x41);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU,2,true);
    TemP=Wire.read()<<8|Wire.read();
    
    Temp[0] = (TemP + 12412.0) / 340.0;
  
   //Mostrar los valores por consola
   Serial.print("Angle X: "); Serial.print(Angle[0]); Serial.println("n");
   Serial.print("Angle Y: "); Serial.print(Angle[1]); Serial.print("n------------n");
   Serial.print("tEMP"); Serial.print(TemP); Serial.print("n-------n");
   Serial.print("tEMP"); Serial.print(Temp[0]); Serial.print("`C");

  
   delay(10); //Nuestra dt sera, pues, 0.010, que es el intervalo de tiempo en cada medida
}


