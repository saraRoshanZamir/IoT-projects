#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Servo.h>

const char* ssid = "NodeNCU";
const char* password = "123456789";

IPAddress local_ip(192, 168, 1, 1);
IPAddress gateway(192, 168, 4, 1);
IPAddress subnet(255, 255, 255, 0);
ESP8266WebServer server(80);

Servo myservo;
#define LED1 D3
const int trigPin = D5;
const int echoPin = D6;
long duration;
int distance;
int voltage;
int charge;
int itsEmpty;
unsigned long previousMoveServoMillis = 0;
unsigned long previousMoveServoByTimerMillis = 0;
unsigned long timerDuration = 0;
int state = 0;
int isTimerOn = 0;

void handle_UpdateUltraSonic() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;
  charge = 100 - ((distance * 100) / 20);
  Serial.println("charge: "+String(charge));
  if (charge < 10) {
    digitalWrite(LED1, HIGH);
  }
  else {
    digitalWrite(LED1, LOW);
  }
  server.send(200, "text/html", SendHTML());
};

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(LED1, OUTPUT);
  myservo.attach(D1);
  Serial.begin(115200);

  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  WiFi.softAP(ssid, password);

  server.on("/", handle_OnConnect);
  server.on("/move", handle_MoveServo);
  server.on("/moveByTimer", handle_MoveServoByTimer);
  server.on("/updateUltraSonic", handle_UpdateUltraSonic);

  server.begin();
}

void loop() {
  server.handleClient();
  int sensorValue = analogRead(A0);
  voltage = sensorValue / 100;

  unsigned long curMillis = millis();
  
  if (state == 1 && (curMillis - previousMoveServoMillis) >= 3000) {
    state = 3;
  }
  else if (state == 2 && (curMillis - previousMoveServoMillis) >= 5000) {
    state = 3;
  }
  if (state == 3) {
    Serial.println("state: "+ String(state));
    state = 0;
    myservo.write(0);
  }
 
  if (isTimerOn == 1 && (curMillis - previousMoveServoByTimerMillis) >= timerDuration) {
    isTimerOn = 0;
    Serial.println("run servo by timer");
    handle_MoveServo();
  }

  delay(1000);
}

void handle_OnConnect() {
  server.send(200, "text/html", SendHTML());
}

void handle_MoveServo() {
  if (state == 0) {
    myservo.write(180);
    if (voltage < 1) {
      previousMoveServoMillis = millis();
      state = 1;
    }
    else {
      previousMoveServoMillis = millis();
      state = 2;
    }
    server.send(200, "text/html", SendHTML());
  }
}

void handle_MoveServoByTimer() {

  if (server.method() == HTTP_GET) {
    String durationStr = server.arg("time");
    Serial.println("durationStr: " + durationStr);
    timerDuration = durationStr.toInt();
    timerDuration *= 1000;
    previousMoveServoByTimerMillis = millis();
    isTimerOn = 1;
    server.send(200, "text/html", SendHTML());
  }
}

String SendHTML() {
  String ptr = " \n";
  ptr += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr += "<title>smart feeder</title>\n";
  ptr += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr += "body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr += ".button {display: block;width: 80px;background-color: #1abc9c;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr += "p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr += "</style>\n";
  ptr += "\n";
  ptr += "\n";
  ptr += "<h3>smart feeder</h3>\n";
  ptr += "<p>Charge level: " + String(charge) + "</p>\n";
  ptr += "<a class=\"button\" href=\"/updateUltraSonic\">Update Charge Level</a>\n";
  ptr += "<a class=\"button\" href=\"/move\">ROTATE</a>\n";
  ptr += "\n";
  ptr += "\n";
  ptr += "<form method=\"get\" action=\"/moveByTimer\">";
  ptr += "<label for=\"time\">Enter time in seconds:</label>";
  ptr += "<br><br>\n";
  ptr += "<input type=\"number\" id=\"time\" name=\"time\" value=\"0\" min=\"0\">";
  ptr += "<input type=\"submit\" value=\"Set Timer\">";
  ptr += "<br><br>\n";
  ptr += "</form>\n";
  return ptr;
}
