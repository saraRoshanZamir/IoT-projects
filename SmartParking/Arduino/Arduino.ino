//arduino uno
#include <Servo.h>
Servo myservo; 
Servo myservo1;
int pos = 0;
const int trigPin = 9;
const int echoPin = 10;
long duration;
int distance;
void setup() {
  myservo.attach(5);
  myservo1.attach(6);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT); 
  Serial.begin(9600); 
}

void loop() {
   digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;
  if(distance<10){
     for (pos = 0; pos <= 180; pos += 1) {
    myservo.write(pos);            
    delay(15);                     

  }

  for (pos = 180; pos >= 0; pos -= 1) { 
    myservo.write(pos);             
    delay(15);                   
  }
  }
  //  if(distance>20){
  //    for (pos = 0; pos <= 180; pos += 1) {
  //   myservo1.write(pos);            
  //   delay(15);                     

  // }

  // for (pos = 180; pos >= 0; pos -= 1) { 
  //   myservo1.write(pos);             
  //   delay(15);                   
  // }
  // }

  // Prints the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.println(distance);
  Serial.println("hello");
  delay(3000);
}

