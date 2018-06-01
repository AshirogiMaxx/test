#include <WiFi.h>


/////////////WEB SERVER //////////////
WiFiServer server(80);

const char WifiPASS[] = "12345679";

void setup(void){

   Serial.begin(115200);
   server.on("/", handleRoot);
   //server.on("/down", handleDownload);
   //server.on("/form", formatear)
   server.on("/inline", [])()
      {
        server,send(200, "text/plain","this works fine ")  
      }
   server.onNotFound(handleNotFound);
  
}

void loop(void){
  
  
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
