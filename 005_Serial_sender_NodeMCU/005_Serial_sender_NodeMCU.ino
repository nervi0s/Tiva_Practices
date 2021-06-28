#include<SoftwareSerial.h>

SoftwareSerial interface(3, 1);

// put your setup code here, to run once:
void setup() {
  interface.begin(9600);
}

void loop() {
  interface.write("0");
  delay(1000);
  interface.write("1");
}
