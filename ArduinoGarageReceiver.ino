const int RELAY_PIN = 2;


void setup() {
  // put your setup code here, to run once:
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(RELAY_PIN, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(RELAY_PIN, HIGH);
  delay(1000);
  digitalWrite(RELAY_PIN, LOW);
  delay(5000);
}
