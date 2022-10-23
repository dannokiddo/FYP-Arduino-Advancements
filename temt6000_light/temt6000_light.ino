// Ambient Light Sensor
#define temt6000pin A0
//int temt6000pin = A0;

void setup() {
  // put your setup code here, to run once:
  pinMode(temt6000pin, INPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  /*float reading = analogRead(temt6000pin); //Read light level
  delay(500);
  
  float square_ratio = reading / 1023.0;      //Get percent of maximum value (1023)
  square_ratio = pow(square_ratio, 2.0);      //Square to make response more obvious

  //analogWrite(LEDPIN, 255.0 * square_ratio);  //Adjust LED brightness relatively
  Serial.println(reading); 
  */

  /*float ambient_read = analogRead(temt6000pin);
  float light_percent = ambient_read/1023;      //percentage
  light_percent = pow(light_percent, 2.0);       //equate

  Serial.println(light_percent);
  */

  
  int light_val = analogRead(temt6000pin);
  float light = light_val * 0.0976;// percentage calculation

  float lux = light_val * 0.9765625;  // 1000/1024
  Serial.println(lux);
  delay(500);
  
}
