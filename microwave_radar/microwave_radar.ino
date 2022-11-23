#define human 12
#define led 7

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

  if (motion == HIGH)
  {
    digitalWrite(led, HIGH);
    Serial.println("Presence");
  }
  else
  {
    digitalWrite(led, LOW);
    Serial.println("Vacant");
  }
}
