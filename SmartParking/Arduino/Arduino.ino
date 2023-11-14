//arduino uno
#include <Servo.h>
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9

byte readCard[4];
String MasterTag = "D3FEFCF6";
String tagID = "";

Servo myservo;
MFRC522 mfrc522(SS_PIN, RST_PIN);
int pos = 0;
const int trigPin = 3;
const int echoPin = 5;
long duration;
int distance;

void setup() {
  myservo.attach(6);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT); 
  SPI.begin();
  Serial.begin(9600); 
  mfrc522.PCD_Init();
  delay(4);
  mfrc522.PCD_DumpVersionToSerial();
  Serial.println(" Access Control ");
  Serial.println("Scan Your Card>>");
}

void handle_moveServo(){
  for (pos = 0; pos <= 90; pos += 1) {
    myservo.write(pos);            
    delay(15);                     

  }
    delay(1000);   
  for (pos = 90; pos >= 0; pos -= 1) { 
    myservo.write(pos);             
    delay(15);                   
  }
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
    handle_moveServo();
  }

  delay(3000);

   while (getID()) {
  
    if (tagID == MasterTag){
      Serial.println(" Access Granted!");
      Serial.println("--------------------------");
      handle_moveServo();

    }

    else{
      Serial.println(" Access Denied!");
      Serial.println("--------------------------");
    }

    delay(2000);
    Serial.println(" Access Control ");
    Serial.println("Scan Your Card>>");
  }
}

boolean getID() 
{
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return false;
  }

  if ( ! mfrc522.PICC_ReadCardSerial()) {
  return false;
  }
  tagID = "";
  for ( uint8_t i = 0; i < 4; i++) {
  tagID.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  
  tagID.toUpperCase();
  mfrc522.PICC_HaltA(); // Stop reading
  return true;

}
