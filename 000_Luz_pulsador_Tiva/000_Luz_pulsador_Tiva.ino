// pines de entrada y salida
int PIN_24_IN = 24;
int PIN_63_OUT = 63;
int PIN_81_OUT = 81;

// varibles de control
bool LED_ON = false;
bool canChange = true;


// the setup routine runs once when you press reset:
void setup() {
  // inicializar pines según su modo
  pinMode(PIN_24_IN, INPUT);
  pinMode(PIN_63_OUT, OUTPUT);
  pinMode(PIN_81_OUT, OUTPUT);

  Serial.begin(9600);
}

// the loop routine runs over and over again forever:
void loop() {
  Serial.println("Chat server addregfdgfdgss:");
  int pin_24_in_value = digitalRead( PIN_24_IN );

  if (pin_24_in_value == LOW) {
    delay(100);
    canChange = true;
  }

  if (pin_24_in_value == HIGH && canChange) {
    LED_ON = !LED_ON;
    canChange = false;

    if (LED_ON) {
      digitalWrite(PIN_63_OUT, HIGH);
      digitalWrite(PIN_81_OUT, HIGH);
      Serial.println("Chat server addregfdgfdgss:");
    } else {
      digitalWrite(PIN_63_OUT, LOW);
      digitalWrite(PIN_81_OUT, LOW);
    }
  }

  /*digitalWrite(82, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(500);               // wait for a second
    digitalWrite(82, LOW);    // turn the LED off by making the voltage LOW
    delay(500);               // wait for a second
  */

}
