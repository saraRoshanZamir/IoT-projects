#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "NodeNCU";
const char* password = "123456789";

IPAddress local_ip(192, 168, 1, 1);
IPAddress gateway(192, 168, 4, 1);
IPAddress subnet(255, 255, 255, 0);
ESP8266WebServer server(80);

int IR = D5;
int IRState=1;

SoftwareSerial mySerial(D1, D2);

void setup() {
  pinMode(IR, INPUT);
  Serial.begin(115200);
  mySerial.begin(9600);

  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  WiFi.softAP(ssid, password);

  server.on("/", handle_OnConnect);
  server.on("/getIRState", handle_getIRState);
  server.onNotFound(handle_NotFound);
  server.begin();
}

void handle_getIRState() {
  String response = String(IRState);
  server.send(200, "text/plain", response);
}

void handle_updateIR() {
  int newIRState = digitalRead(IR);
  if (newIRState != IRState) {
    IRState = newIRState;
    server.send(200, "text/plain", String(IRState));
    Serial.println(IRState);
  }
}

void loop() {
  server.handleClient();
  //String msg = mySerial.readStringUntil('\r');
  //Serial.println(msg);
  handle_updateIR();
  delay(1);
}


void handle_OnConnect() {
  String html = "<!DOCTYPE html>\n";
  html += "<html>\n";
  html += "<head>\n";
  html += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  html += "<title>smart Parking</title>\n";
  html += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  html += "body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  html += "p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  html += "</style>\n";
  html += "<script>\n";
  html += "function refreshIRState() {\n";
  html += "  var xhttp = new XMLHttpRequest();\n";
  html += "  xhttp.onreadystatechange = function() {\n";
  html += "    if (this.readyState == 4 && this.status == 200) {\n";
  html += "      var irState = parseInt(this.responseText);\n";
  html += "      var parkingStatus = (irState === 0) ? 'full' : 'empty';\n";
  html += "      document.getElementById('irState').textContent = parkingStatus;\n";
  html += "    }\n";
  html += "  };\n";
  html += "  xhttp.open('GET', '/getIRState', true);\n";
  html += "  xhttp.send();\n";
  html += "}\n";
  html += "setInterval(refreshIRState, 1000);\n";
  html += "</script>\n";
  html += "</head>\n";
  html += "<body>\n";
  html += "<h3>smart Parking</h3>\n";
  html += "<p>Parking 1 is: <span id=\"irState\"></span></p>\n";
  html += "</body>\n";
  html += "</html>\n";

  server.send(200, "text/html", html);
}

void handle_NotFound() {
  server.send(404, "text/plain", "Not found");
}