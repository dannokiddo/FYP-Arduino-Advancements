// include libraries
#include <DHT.h>    //temp & humid
#include <IRremote.h>

// define pins 
#define temt6000pin A0
#define dhtpin      2   
#define DHTTYPE DHT11
#define transpin    3

//define identifiers
DHT dht(dhtpin, DHTTYPE);

void setup() {
    // Begin Serial Monitor
    Serial.begin(9600);

    dht.begin();
    IRsender.begin(transpin);
    
    // pinMode define
    pinMode(temt6000pin, INPUT);
}

void loop() {
  
  ambient();

  dhtsense();
  
  Serial.println("\n**********************************\n");

  if (hic >= 28) {
    IrSender.sendNEC(0x, 32); //turn AC on
    delay(1000);
    IrSender.sendNEC(0x, 32); //
  }
}

void ambient() {
  // ambient light
  int light_val = analogRead(temt6000pin);
  float lux = light_val * 0.9765625;  // 1000/1024
  Serial.println("Luminance Lux: " + (String)lux + "\n");
  delay(500);
}

void dhtsense() {
  // temp & humid //readcan take 250ms -  2s
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);
  
  if (isnan(h) || isnan(t) || isnan(f)) { //if read fail then exit
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  float hic = dht.computeHeatIndex(t, h, false); //false->notFarhrenheit = Celcius

  Serial.println("Humidity   : " + (String)h + " %");
  Serial.println("Temperature: " + (String)t + " °C");
  Serial.println("Heat Index : " + (String)hic + " °C \n");
  delay(500);
}
