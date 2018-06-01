/*
 *  This sketch demonstrates how to use ESP8266 to control Xiaomi Yi Camera.
 *
 * @author  Emmanuel Granatello
 * @version 0.1
 * @since   2016-02-16 
 */
 
#include "ESP8266WiFi.h"
 
 
WiFiClient client;

void setup() {
    // Only For debug
    Serial.begin(115200);
}
 
String searchCameraSSID() {
    String cameraSSID;
 
    // Set WiFi to station mode and disconnect from an AP if it was previously connected
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);
 
    // WiFi.scanNetworks will return the number of networks found
    int n = WiFi.scanNetworks();
    if (n > 0) {
        for (int i = 0; i < n; ++i) {
            // Search Camera network
            if (WiFi.SSID(i).startsWith("YDXJ_")) {
                cameraSSID = WiFi.SSID(i);
                Serial.println("SSID Found");
                break;
            }
            delay(10);
        }
    }
 
    return cameraSSID;
}
 
bool connectToCamera(String SSID) {
    bool result = true;
    short retry = 30;
    char ssid[30];
    char password[11] = "1234567890";
 
    // String to Char*
    SSID.toCharArray(ssid, SSID.length() + 1);
 
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        if (retry == 0) {
            result = false;
            break;
        }
        delay(500);
        retry--;
    }
 
    return result;
}
 
bool connectToServerRC() {
    bool result = true;
    const int jsonPort = 7878;
 
    // Open Tcp connection
    if (!client.connect("192.168.42.1", jsonPort)) {
        result = false;
        
    }
    Serial.println("Webserver Connected");
 
    return result;
}
 
String requestToken() {
    String token;
    // This will send the request token msg to the server
    client.print("{\"msg_id\":257,\"token\":0}\n\r");
    delay(1000);
 
    // Read all the lines of the reply from server and print them to Serial
    String response;
    while (client.available()) {
        char character = client.read();
        response.concat(character);
        Serial.println(character);
    }
 
    // Search token in to the stream
    int offset = response.lastIndexOf(':');
    if (offset != -1) {
        for (int i = offset + 1; i < response.length(); ++i) {
            if ((response.charAt(i) != ' ') && (response.charAt(i) != '}')) {
                token.concat(response.charAt(i));
            }
        }
    }
    Serial.println(token);
 
    return token;
}
 
void shoot(String token) {
    client.print("{\"msg_id\":769,\"token\":");
    client.print(token);
    client.print("}\n\r");
}
 
void loop() {
    String cameraSSID = searchCameraSSID();
 
    if (cameraSSID.length() != 0) {
        if (!connectToCamera(cameraSSID)) {
            return;
        }
 
        if (!connectToServerRC()) {
            return;
        }
 
        String token = requestToken();
        if (token.length() != 0) {
            shoot(token);
        }
    }
  }
