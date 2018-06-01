#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>


const char* ssid     = "AXTEL XTREMO-0C58";
const char* password = "036F0C58";

ESP8266WebServer server(80);

void handleRoot() {

  String color, finale;
  String hexstring = "#FF3Fa0";

  if (server.hasArg("chroma")) {
    Serial.print("Variable Exists: ");
    color = server.arg("chroma");
    Serial.println(color);
    //String test = color;
    //color.remove(0,1);
    //Serial.println(color);
    long number = strtol( &color[1], NULL, 16);
    Serial.println(number);
    long r = number >> 16;
    long g = number >> 8 & 0xFF;
    long b = number & 0xFF;
    Serial.println(r);
    Serial.println(g);
    Serial.println(b);
  }

  String temp = "<html>";
  temp += "<body><form action='/' method='POST'>";
  temp += "<p>Change the color of the color picker, and then click the button below.</p>";
  temp += "Color picker: <input type='color' name='chroma' id='myColor' value='#ff0080'>";
  temp += "<input type='submit' name='SUBMIT' value='Submit'></form>";
  temp += "<p>Note that the default value is not affected when you change the value of the color picker.</p>";
  temp += "<button type='button' onclick='myFunction()'>Try it</button>";
  temp += "<p><b>Note:</b> type='color' is not supported in Internet Explorer 11 and earlier versions or Safari 9.1 and earlier versions.</p>";
  temp += "<p id='demo'></p>";
  temp += "<script>";
  temp += "function myFunction() {";
  temp += "var x = document.getElementById('myColor');";
  temp += "    var defaultVal = x.defaultValue;";
  temp += "    var currentVal = x.value;";

  temp += "    if (defaultVal == currentVal) {";
  temp += "        document.getElementById(demo').innerHTML = 'Default value and current value is the same: '";
  temp += "        + x.defaultValue + ' and ' + x.value";
  temp += "        + '<br>Change the color of the color picker to see the difference!';";
  temp += "    } else {";
  temp += "        document.getElementById('demo').innerHTML = 'The default value was: ' + defaultVal";
  temp += "        + '<br>The new, current value is: ' + currentVal;";
  temp += "    }";
  temp += "}";
  temp += "</script>";
  temp += "</body>";
  temp += "</html>";



  server.send(200, "text/html", temp);
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void setup() {
  Serial.begin(115200);

  delay(1000);

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.onNotFound(handleNotFound);
  server.begin();
}

void loop() {
  server.handleClient();
}
