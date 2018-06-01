
String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete

void setup() {
  // initialize serial:
  Serial.begin(9600);

  Serial3.begin(9600);
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
      Serial.println("hola"); 
      Serial3.println("AT");

}

void loop() {
  // print the string when a newline arrives:
  if (stringComplete) {
    Serial.println("switching on");

    Serial.println(inputString); 
    if (inputString == "RING"){
    Serial.println("switching on");
    }
    if (inputString == "OK"){
    Serial.println("face");
    }
    // clear the string:
    inputString = "";
    stringComplete = false;
  }
}

/*
  SerialEvent occurs whenever a new data comes in the
 hardware serial RX.  This routine is run between each
 time loop() runs, so using delay inside loop can delay
 response.  Multiple bytes of data may be available.
 */
void serialEvent1() {
  while (Serial3.available()) {
    // get the new byte:
    char inChar = (char)Serial3.read(); 
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    } 
   
  }
  
  inputString.trim();
}
