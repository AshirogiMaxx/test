int8_t answer;
int onModulePin= 2;
char aux_string[30];
int flag = 0;
char number [20];
char realnumber[10];
char mynumber[10];
int a=0;
int b=0;
int c=0;
//Your phone number
char phone_number[]="4499800433";  

char data[100];
int data_size;

char aux_str[30];
char aux;
int x = 0;
char N_S,W_E;
int pin = 13;

char url[] = "pruebas.libelium.com";
char frame[200];

char latitude[15];
char longitude[15];
char altitude[6];
char date[16];
char time[7];
char satellites[3];
char speedOTG[10];
char course[10];

void setup(){
    mynumber[0]='4';
    mynumber[1]='4';
    mynumber[2]='9';
    mynumber[3]='9';
    mynumber[4]='8';
    mynumber[5]='0';
    mynumber[6]='0';
    mynumber[7]='4';
    mynumber[8]='3';
    mynumber[9]='3';
    

    pinMode(onModulePin, OUTPUT);
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
    Serial.begin(9600);    
    
    power_on();

    delay(5000);
    
    sendATcommand("AT+CPIN=1111", "OK", 2000);  
  
    delay(3000);  

    while( (sendATcommand("AT+CREG?", "+CREG: 0,1", 1000) || sendATcommand("AT+CREG?", "+CREG: 0,5", 1000)) == 0 );
    sendATcommand("AT+CLIP=1", "OK", 1000); 
    
}
void loop(){
    
    answer = sendATcommand("", "+CLIP", 1000);
    //Detect incomming call
    if (answer == 1)
    {
        Serial.println("Incoming call");          
          
          if ( flag == 0){
            
              for (int i=0; i<19; i++){
               // read the incoming byte:
               while (Serial.available() == 0)
               {                 
                 delay (50);
               }
               //Stores phone number
               number[i] = Serial.read();  
               
              }
              Serial.flush();
              flag = 1; 
          }          
          //Stores phone calling number
          for (int i=0; i<=14; i++){
            if(number[i]== '"'){
              i++;
              realnumber[0]=number[i];
              i++;
              realnumber[1]=number[i];
              i++;
              realnumber[2]=number[i];
              i++;
              realnumber[3]=number[i];
              i++;
              realnumber[4]=number[i];
              i++;
              realnumber[5]=number[i];
              i++;
              realnumber[6]=number[i];
              i++;
              realnumber[7]=number[i];
              i++;
              realnumber[8]=number[i];
              i++;
              realnumber[9]=number[i];
              break;
            }            
    }
    //Check phone number
    for (int i=0;i<10;i++){ 
      if (realnumber[i] == mynumber[i]){
          a++;       
          if( a==10){  
             Serial.println("Correct number");
             sendATcommand("ATH", "OK", 1000);
             digitalWrite(pin, HIGH);
             Serial.println(realnumber);
               if(b==1){
                  b=0;
                }else{                  
                  b=1;
                  c=1;
                    }
                    break;
                  }
                }else{             
                  Serial.println("Wrong number");
                  Serial.println(realnumber);
                  break;
                 }
              }          
     a=0;
     answer=0;
     flag = 0;     
}
}


void power_on(){

    uint8_t answer=0;

    digitalWrite(onModulePin,HIGH);
    delay(3000);
    digitalWrite(onModulePin,LOW);

    while(answer == 0){     // Send AT every two seconds and wait for the answer
        answer = sendATcommand("AT", "OK", 2000);    
    }
}

int8_t sendATcommand(char* ATcommand, char* expected_answer, unsigned int timeout){

    uint8_t x=0,  answer=0;
    char response[100];
    unsigned long previous;

    memset(response, '\0', 100);    // Initialice the string
    
    delay(100);
    
    while( Serial.available() > 0) Serial.read();    // Clean the input buffer
    
    if (ATcommand[0] != '\0')
    {
        Serial.println(ATcommand);    // Send the AT command 
    }


    x = 0;
    previous = millis();

    // this loop waits for the answer
    do{
        if(Serial.available() != 0){    // if there are data in the UART input buffer, reads it and checks for the asnwer
            response[x] = Serial.read();
            //Serial.print(response[x]);
            x++;
            if (strstr(response, expected_answer) != NULL)    // check if the desired answer (OK) is in the response of the module
            {
                answer = 1;
            }
        }
    }while((answer == 0) && ((millis() - previous) < timeout));    // Waits for the asnwer with time out

    return answer;
}

