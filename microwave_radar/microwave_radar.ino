#define human 21
#define led 13

void setup() {
  // put your setup code here, to run once:
  pinMode(human, INPUT);
  pinMode(led, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  int motion = digitalRead(human);
  delay(500);

  Serial.println(motion);

  if (motion > 0)
  {
    digitalWrite(led, HIGH);
    Serial.println("Presence");
  }
  else
  {
    digitalWrite(led, LOW);
    Serial.println("Vacant");
  }
  delay(100);
}
