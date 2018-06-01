float DCM_Matrix [3][3] = {{2, 3, 4}, {3, 4, 1}, {3, 2, 5}};
float Update_Matrix [3][3]= {{1, 3, 9}, {0, 4, 9}, {7, 5, 5}};
float Temporary_Matrix [3][3];


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  Matrix_Multiply(DCM_Matrix,Update_Matrix,Temporary_Matrix); //a*b=c

}


void Matrix_Multiply(float a[3][3], float b[3][3],float mat[3][3])
{

  for(int f=0; f<3; f++)
  {
    for(int g=0; g<3; g++)
    {
      Serial.print(a[f][g]);Serial.print(" ");
    }
    Serial.print("\n");
  }
  float op[3]; 
  for(int x=0; x<3; x++)
  {
    for(int y=0; y<3; y++)
    {
      for(int w=0; w<3; w++)
      {
       op[w]=a[x][w]*b[w][y];
      } 
      mat[x][y]=0;
      mat[x][y]=op[0]+op[1]+op[2];
      
      float test=mat[x][y];
      Serial.print(test);Serial.print(" ");
    }
    Serial.print("\n");
  }
}
