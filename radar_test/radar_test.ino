int flag = 0;
int Sensor = 12;

void setup()

{
  Serial.begin(9600);
  pinMode (Sensor, INPUT);

  Serial.println("Waiting for motion");
}
void loop()
{
  int val = digitalRead(Sensor);

  if ((val > 0) && (flag == 0))
  {
    Serial.println("Motion Detected");
    flag = 1;
    delay(500);
  }
  if (val == 0)
  {
    Serial.println("NO Motion");
    flag = 0;
    delay(500);
  }
}