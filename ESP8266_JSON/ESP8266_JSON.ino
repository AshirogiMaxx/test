#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const char* ssid = "TP-LINK_A66676";
const char* password = "04566014";


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while(WiFi.status() != WL_CONNECTED){
     delay(1000);
     Serial.println("Connecting...");
    
    }
}

void loop() {
  // put your main code here, to run repeatedly:
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http; // object class HTTPClient
    http.begin("http://jsonplaceholder.typicode.com/users/1");
    int httpCode = http.GET();
    //Check the returning code
    if(httpCode > 0){
      const size_t bufferSize = JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(5) + JSON_OBJECT_SIZE(8) + 370;
      DynamicJsonBuffer jsonBuffer(bufferSize);
      JsonObject& root = jsonBuffer.parseObject(http.getString());
      // Parameters
      int id = root["id"]; // 1
      const char* name = root["name"]; // "Leanne Graham"
      const char* username = root["username"]; // "Bret"
      const char* email = root["email"]; // "Sincere@april.biz"
      // Output to serial monitor
      Serial.print("Name:");
      Serial.println(name);
      Serial.print("Username:");
      Serial.println(username);
      Serial.print("Email:"); 
      Serial.println(email);
      }
      http.end();
    //Check the returning code
      }
      delay(6000);
  }

