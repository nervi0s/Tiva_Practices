#include<SoftwareSerial.h>
#include <ESP8266WiFi.h>

SoftwareSerial interface(3, 1);
const char *ssid =  "AndroidAP3025";     // replace with your wifi ssid and wpa2 key
const char *pass =  "efa1ee8605e4";

void setup() {
  interface.begin(9600);
  //Serial.begin(9600);               // Start the Serial communication to send messages to the computer
  WiFi.begin(ssid, pass);             // Connect to the network
  //Serial.print("Connecting to ");
  //Serial.print(ssid); Serial.println(" ...");

  int i = 0;
  while (WiFi.status() != WL_CONNECTED) { // Wait for the Wi-Fi to connect
    delay(1000);
    //Serial.print(++i); Serial.print(' ');
  }

  //Serial.println('\n');
  //Serial.println("Connection established!");
  //Serial.print("IP address:\t");
  //Serial.println(WiFi.localIP());         // Send the IP address of the ESP8266 to the computer
  // put your setup code here, to run once:
}

void loop() {
  //Serial.println("Connection established!");
  interface.write("0");
  delay(1000);
  interface.write("1");
  delay(1000);

}
