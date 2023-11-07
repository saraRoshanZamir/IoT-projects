#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Arduino.h>
#include <SoftwareSerial.h>
 
/* Put your SSID & Password */
const char* ssid = "NodeNCU";  // Enter SSID here
const char* password = "123456789";  //Enter Password here
 
/* Put IP Address details */
IPAddress local_ip(192,168,1,1);
IPAddress gateway(192,168,4,1);
IPAddress subnet(255,255,255,0);
 
ESP8266WebServer server(80);
 

 const int trigPin = D5;   
 const int echoPin = D6;   
 long duration;  
 int distance;  
 void setup() {  
 pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output  
 pinMode(echoPin, INPUT); // Sets the echoPin as an Input  
 Serial.begin(9600); // Starts the serial communication  
 }  
 void loop() {  
 // Clears the trigPin  
 digitalWrite(trigPin, LOW);  
 delayMicroseconds(2);  
 // Sets the trigPin on HIGH state for 10 micro seconds  
 digitalWrite(trigPin, HIGH);  
 delayMicroseconds(10);  
 digitalWrite(trigPin, LOW);  
 // Reads the echoPin, returns the sound wave travel time in microseconds  
 duration = pulseIn(echoPin, HIGH);  
 // Calculating the distance  
 distance= duration*0.034/2;  
 // Prints the distance on the Serial Monitor  
 Serial.print("Distance: ");  
 Serial.println(distance);  
 delay(2000);  
 }   
