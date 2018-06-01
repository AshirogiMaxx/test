#include <ESP8266WiFi.h>

const char* ssid = "AXTEL-5352";
const char* password = "C3655352";

WiFiServer server(80);


void setup()
{
       Serial.begin(115200);
       delay(1500);

       pinMode(13, OUTPUT);
       digitalWrite(13, LOW);

       Serial.println();
       Serial.println();
       Serial.println("Conencting to: ");
       Serial.println(ssid);

       WiFi.begin(ssid,password);

       while (WiFi.status() != WL_CONNECTED){
          delay(500);
          Serial.println(":");       
       }
       Serial.println("");
       Serial.println("Wifi connected");

       server.begin();
       Serial.println("Server Started");

       Serial.println(WiFi.localIP());       

}

void loop()
{
  WiFiClient client = server.available();
  if(!client){
    return;
  }

  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }

  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();

  int val;
  if  (req.indexOf("/gpio/0") != -1)
    val = 0;
  else if (req.indexOf("/gpio/1") != -1)
    val = 1;
   else {
      Serial.println("invalid request");
      client.stop();
      return;
   }

  digitalWrite(13, val);

  client.flush();

    String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\nGPIO is now ";
  s += (val)?"high":"low";
  s += "</html>\n";

  // Send the response to the client
  client.print(s);
  delay(1);
  Serial.println("Client disonnected");

  // The client will actually be disconnected 
  // when the function returns and 'client' object is detroyed
}

