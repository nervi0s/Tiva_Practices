void setup() {
  pinMode(81, OUTPUT);
  pinMode(82, OUTPUT);

  Serial.begin(9600);
  while (!Serial) {
    Serial.println("waiting...");
    delay(500);
  }
  Serial.println("Connected");
}

void loop() {
  /*
     delay(2000);
     Serial.println("lol");
  */
  if (Serial.available()) {
    int recieved = Serial.parseInt();

    if (recieved == 1) {
      digitalWrite(81, HIGH);
      Serial.println("lol");
    }

    if (recieved == 0) {
      digitalWrite(81, LOW);
      digitalWrite(82, LOW);
      Serial.println("lil");
    }

    if (recieved == 3) {
      digitalWrite(82, HIGH);
      Serial.println("lul");
    }
  }
}
