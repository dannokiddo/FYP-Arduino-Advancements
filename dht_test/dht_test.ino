#include <DHT.h>    //temp & humid

#define dhtpin      2   
#define DHTTYPE DHT11

DHT dht(dhtpin, DHTTYPE);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  dht.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  // temp & humid //readcan take 250ms -  2s
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);
  
  if (isnan(h) || isnan(t) || isnan(f)) { //if read fail then exit
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  float hic = dht.computeHeatIndex(t, h, false); //false->notFarhrenheit = Celcius

  Serial.println("Humidity   : " + (String)h + "%");
  Serial.println("Temperature: " + (String)t + "°C");
  Serial.println("Heat Index : " + (String)hic + "°C \n");
  delay(500);
  
  //Serial.println("Humidity: %d %", h);  //%d is double
  /*Serial.println(F("Humidity: ")); Serial.println(h); //defaultcode
  Serial.println(F("Temperature: ")); Serial.println(t);*/
}
