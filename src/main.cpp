#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

const char* ssid = "esp8266";
const int led = 2;

ESP8266WebServer server(80);

void handleNotFound(){
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}


/* Go to http://192.168.4.1 in a web browser */

int counter = 0;
void handleRoot() {
  static char buff[1000] = {};
  long led_state = 1;
  if(server.hasArg("led")) {
    led_state = server.arg("led").toInt();
  }

  sprintf(
    buff,
    "<h1> welcome to esp8266 </br>"
    "<p> page access count = %d </p><br/>"
    "SET LED <a href='?led=%d'>TO <b>%d</b></a>"
    "</h1>"
    ,
    ++counter,
    !led_state,
    led_state
  );

  server.send(200, "text/html", buff);
  digitalWrite(led, led_state);
}

void setup() {
  delay(1000);
  pinMode(led, OUTPUT);

  // Serial.begin(115200);
  // Serial.println();
  // Serial.print("Configuring access point...");

  WiFi.softAP(ssid);
  IPAddress myIP = WiFi.softAPIP();

  // Serial.print("AP IP address: ");
  // Serial.println(myIP);

  server.on("/", handleRoot);
  server.onNotFound(handleNotFound);

  server.begin();
  // Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
  delay(500);
}