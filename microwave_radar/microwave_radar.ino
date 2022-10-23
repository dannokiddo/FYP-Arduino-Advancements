#define human A0
#define led 7

void setup() {
  // put your setup code here, to run once:
  pinMode(human, INPUT);
  pinMode(led, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  int motion = analogRead(human);
  delay(100);

  Serial.println(motion);

  if (motion == HIGH)
  {
    digitalWrite(led, HIGH);
  }
  else
  {
    digitalWrite(led, LOW);
  }
}
