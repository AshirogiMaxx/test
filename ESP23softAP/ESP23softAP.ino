#include <WiFi.h>
#include <ESP32WebServer.h>
#include <WiFiClient.h>
#include <ESPmDNS.h>

float   t = 2.1;
float   h = 2.2;
float   p = 2.3;
/////////////WEB SERVER //////////////

String  etatGpio[4] = {"OFF","OFF","OFF","OFF"};

int Amarillo = 16;//16
int AntesAmllo;
int ActualAmllo;
int contadorA1 = 0;

int Verde = 14;//14
int AntesVerde;
int ActualVerde;
int contadorV1 = 0;

int TimeGlobal;
int GlobalTimeAcumSeg;
int GlobalTimeAcumMin;
int GlobalTimeAcumHor;
int TimeAntesAmllo;
int TimeActualAmllo;
int TimeONamarilloseg;
int TimeONamarillomin;
int TimeONamarillohor;
int TimeOFFamarilloseg;
int TimeOFFamarillomin;
int TimeOFFamarillohor;
int TimeAcumAmlloOFFseg;
int TimeAcumAmlloOFFmin;
int TimeAcumAmlloOFFhor;
int TimeAcumAmlloONseg;
int TimeAcumAmlloONmin;
int TimeAcumAmlloONhor;
int TimeAntesVerde;
int TimeActualVerde;
int TimeONverdeseg;
int TimeONverdemin;
int TimeONverdehor;
int TimeOFFverdeseg;
int TimeOFFverdemin;
int TimeOFFverdehor;
int TimeAcumVerdeOFFseg;
int TimeAcumVerdeOFFmin;
int TimeAcumVerdeOFFhor;
int TimeAcumVerdeONseg;
int TimeAcumVerdeONmin;
int TimeAcumVerdeONhor;
int datedia;
int datemes;
int dateyear;
int HorDif;  
int MinDif;
int SegDif; 
int tam_file;
int porce;

ESP32WebServer server(80);

const char* ssid     = "ARRIS-DC22";
const char* password = "DDE8E9ACB601667D";

String getPage(){
  String page = "<html lang='fr'><head><meta http-equiv='refresh' content='60' name='viewport' content='width=device-width, initial-scale=1'/>";
  page += "<link rel='stylesheet' href='https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css'><script src='https://ajax.googleapis.com/ajax/libs/jquery/3.1.1/jquery.min.js'></script><script src='https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/js/bootstrap.min.js'></script>";
  page += "<title>ESP8266 Demo - www.projetsdiy.fr</title></head><body>";
  page += "<div class='container-fluid'>";
  page +=   "<div class='row'>";
  page +=     "<div class='col-md-12'>";
  page +=       "<h1>Demo Webserver ESP8266 + Bootstrap</h1>";
  page +=       "<h3>Mini station m&eacute;t&eacute;o</h3>";
  page +=       "<ul class='nav nav-pills'>";
  page +=         "<li class='active'>";
  page +=           "<a href='#'> <span class='badge pull-right'>";
  page +=           t;
  page +=           "</span> Temp&eacute;rature</a>";
  page +=         "</li><li>";
  page +=           "<a href='#'> <span class='badge pull-right'>";
  page +=           h;
  page +=           "</span> Humidit&eacute;</a>";
  page +=         "</li><li>";
  page +=           "<a href='#'> <span class='badge pull-right'>";
  page +=           p;
  page +=           "</span> Pression atmosph&eacute;rique</a></li>";
  page +=       "</ul>";
  page +=       "<table class='table'>";  // Tableau des relevés
  page +=         "<thead><tr><th>Capteur</th><th>Mesure</th><th>Valeur</th><th>Valeur pr&eacute;c&eacute;dente</th></tr></thead>"; //Entête
  page +=         "<tbody>";  // Contenu du tableau
  page +=           "<tr><td>DHT22</td><td>Temp&eacute;rature</td><td>"; // Première ligne : température
  page +=             t;
  page +=             "&deg;C</td><td>";
  page +=             "-</td></tr>";
  page +=           "<tr class='active'><td>DHT22</td><td>Humidit&eacute;</td><td>"; // 2nd ligne : Humidité
  page +=             h;
  page +=             "%</td><td>";
  page +=             "-</td></tr>";
  page +=           "<tr><td>BMP180</td><td>Pression atmosph&eacute;rique</td><td>"; // 3ème ligne : PA (BMP180)
  page +=             p;
  page +=             "mbar</td><td>";
  page +=             "-</td></tr>";
  page +=       "</tbody></table>";
  page +=       "<h3>GPIO</h3>";
  page +=       "<div class='row'>";
  page +=         "<div class='col-md-4'><h4 class ='text-left'>D5 ";
  page +=           "<span class='badge'>";
  page +=           etatGpio[0];
  page +=         "</span></h4></div>";
  page +=         "<div class='col-md-4'><form action='/' method='POST'><button type='button submit' name='D5' value='1' class='btn btn-success btn-lg'>ON</button></form></div>";
  page +=         "<div class='col-md-4'><form action='/' method='POST'><button type='button submit' name='D5' value='0' class='btn btn-danger btn-lg'>OFF</button></form></div>";
  page +=         "<div class='col-md-4'><h4 class ='text-left'>D6 ";
  page +=           "<span class='badge'>";
  page +=           etatGpio[1];
  page +=         "</span></h4></div>";
  page +=         "<div class='col-md-4'><form action='/' method='POST'><button type='button submit' name='D6' value='1' class='btn btn-success btn-lg'>ON</button></form></div>";
  page +=         "<div class='col-md-4'><form action='/' method='POST'><button type='button submit' name='D6' value='0' class='btn btn-danger btn-lg'>OFF</button></form></div>";
  page +=         "<div class='col-md-4'><h4 class ='text-left'>D7 ";
  page +=           "<span class='badge'>";
  page +=           etatGpio[2];
  page +=         "</span></h4></div>";
  page +=         "<div class='col-md-4'><form action='/' method='POST'><button type='button submit' name='D7' value='1' class='btn btn-success btn-lg'>ON</button></form></div>";
  page +=         "<div class='col-md-4'><form action='/' method='POST'><button type='button submit' name='D7' value='0' class='btn btn-danger btn-lg'>OFF</button></form></div>";
  page +=         "<div class='col-md-4'><h4 class ='text-left'>D8 ";
  page +=           "<span class='badge'>";
  page +=           etatGpio[3];
  page +=         "</span></h4></div>";
  page +=         "<div class='col-md-4'><form action='/' method='POST'><button type='button submit' name='D8' value='1' class='btn btn-success btn-lg'>ON</button></form></div>";
  page +=         "<div class='col-md-4'><form action='/' method='POST'><button type='button submit' name='D8' value='0' class='btn btn-danger btn-lg'>OFF</button></form></div>";
  page +=       "</div>";
  page +=     "<br><p><a href='http://www.projetsdiy.fr'>www.projetsdiy.fr</p>";
  page += "</div></div></div>";
  page += "</body></html>";
  return page;
}

void handleRoot() {
  //Serial.println("Conectado a html");
  char temp[2500];
    snprintf (temp, 2500,
          "<html>\
            <head>\
            <meta http-equiv='refresh' content='1'/>\
            <title>\Torreta ESP</title>\
            <STYLE type='text/css'>\
            H1 { text-align: center}\
            </STYLE>\
            <style>\
            body { background-color: #000000; font-family: Arial, Helvetica, Sans-Serif; Color: #FFFFFF; }\
            </style>\
            <script type='text/javascript'>\
            function ConfirmDemo() {\
            var mensaje = confirm('Al iniciar un nuevo historial se borraran los datos anteriores Desea continual?');\
            if (mensaje) {document.historial.submit();}\
            else {alert('Seguimiento historial')}\
            }\
            </script>\
            </head>\
            <body>\
              <h1>TORRETA</h1>\
              <style>\
              div   { text-align: center; }\
              table { margin: auto; }\
              </style>\
              <table>\
              </tr>\
              <th>\Memoria utilizada:</th>\
              <td>\%02d%</td>\
              </tr>\
              </table>\
              </div>\
              <table>\
                <tr>\
                  <th>\Color de torreta</th>\
                  <th>\Estado</th>\
                  <th>\Fecha</th>\
                  <th>\Veces Encendido</th>\
                  <th>\Hora encendido</th>\
                  <th>\Hora apagado</th>\
                  <th>\Tiempo Encendido</th>\
                </tr>\
                <tr>\
                  <td>\Amarillo</td>\
                  <td>\%01d</td>\
                  <td>\%02d/%02d/%02d</td>\
                  <td>\%01d</td>\
                  <td>\%02d:%02d:%02d</td>\
                  <td>\%02d:%02d:%02d</td>\
                  <td>\%02d:%02d:%02d</td>\
                </tr>\
                <tr>\
                  <td>\Verde</td>\
                  <td>\%01d</td>\
                  <td>\%02d/%02d/%02d</td>\
                  <td>\%01d</td>\
                  <td>\%02d:%02d:%02d</td>\
                  <td>\%02d:%02d:%02d</td>\
                  <td>\%02d:%02d:%02d</td>\
                </tr>\
              </table>\
              <div>\
              <style type='text/css'>\
              <!-- .centrar { text-align:center; }-->\
              </style>\
              <a href='/down'>Descargar historial</a>\
              <form name=historial action='/form'>\
              <input type='button' onclick='ConfirmDemo()' value='Nuevo historial' />\
              </form>\
            </body>\
            </html>"
         ,porce
         ,ActualAmllo, datedia, datemes, dateyear, contadorA1, TimeONamarillohor, TimeONamarillomin, TimeONamarilloseg,TimeOFFamarillohor, TimeOFFamarillomin,TimeOFFamarilloseg, TimeAcumAmlloONhor, TimeAcumAmlloONmin, TimeAcumAmlloONseg
         ,ActualVerde, datedia, datemes, dateyear, contadorV1, TimeONverdehor, TimeONverdemin, TimeONverdeseg, TimeOFFverdehor, TimeOFFverdemin, TimeOFFverdeseg, TimeAcumVerdeONhor, TimeAcumVerdeONmin, TimeAcumVerdeONseg
        );
   server.send ( 200, "text/html", temp );
}

void handleNotFound(){
    String message = "File Not Found\n\n";
    server.send(404, "text/plain", message);
  }
  
void handleDownload(){
  int32_t time = millis();

  String Webstring = "";
  Webstring += "HTTP/1.1 200 OK\r\n";
  Webstring += "Content-Type: text/plain\r\n";
  Webstring += "Content-Disposition: attachment; filename=\"datalog.csv\"\r\n";
  Webstring += "Content-Length: " + String(time) + "\r\n";
  Webstring += "\r\n";
  server.sendContent(Webstring);
  time = millis() - time;
  Serial.print(time); Serial.println(" ms elapsed");
  }

void formatear(){
  server.send(200, "text/html", getPage());
  
  }



void setup(){
    Serial.begin(115200);
    WiFi.begin(ssid, password);
    Serial.println(""); 
    while(WiFi.status() != WL_CONNECTED){
      delay(500);
      Serial.print(".");
    }
    Serial.println("");
    Serial.println("Connected to ");
    Serial.println(ssid);
    Serial.print("IP Adress: ");
    Serial.println(WiFi.localIP());

    if(MDNS.begin("esp32")){
        Serial.println("MDNS responder started");
      }
      
    server.on("/", handleRoot);
    server.on("/down", handleDownload);
    server.on("/form", formatear);

    
    server.onNotFound(handleNotFound);
    

    server.begin();
    Serial.println("HTTP server started");
}

void loop(){
  server.handleClient();
  }
