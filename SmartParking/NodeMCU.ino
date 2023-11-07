//nodemcu
#include <SoftwareSerial.h>
 SoftwareSerial mySerial (D1, D2);
void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);
mySerial.begin(9600);
}

void loop() {
  String msg=mySerial.readStringUntil('\r');
  Serial.println(msg);


}
