// put your setup code here, to run once:
void setup() {
  pinMode(7, INPUT);
  Serial.begin(9600);
}

// put your main code here, to run repeatedly:
void loop() {
  Serial.println(analogRead(7));
  delay(500);
}
