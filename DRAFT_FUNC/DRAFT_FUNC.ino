
// include libraries
#include <DHT.h>      //temp & humid
#include <IRremote.h>
#include <SPI.h>      //wifi **Week6
#include <WiFiNINA.h> 
#include <WiFiUdp.h>  //ntp signal
#include <RTCZero.h>  //real time clock **

// define pins 
#define temt6000pin A0
#define dhtpin      2   
#define DHTTYPE DHT11
#define transpin    3

//define identifiers
DHT dht(dhtpin, DHTTYPE);
RTCZero rtc;

int status = WL_IDLE_STATUS;      // Wifi status
char ssid[] = "Nazrin's Family";  // Wifi SSID
char pass[] = "cheesecake6";      // Wifi password

const int GMT = +8;               // Time zone constant

void setup() {
  // Begin Serial Monitor
  Serial.begin(9600);

  WiFiConnect();
  printWiFiStatus();

  rtc.begin();
  dht.begin();
  IrSender.begin(transpin);
  
  // pinMode define
  pinMode(temt6000pin, INPUT);

  //NTP Fetch Service
  unsigned long epoch;                  // Variable to represent epoch
  int numberOfTries = 0, maxTries = 6;  // Variable for number of tries to NTP service

    do {    // Get epoch
      epoch = WiFi.getTime();
      numberOfTries++;
    }

    while ((epoch == 0) && (numberOfTries < maxTries));

      if (numberOfTries == maxTries) {
      Serial.print("NTP unreachable!!");
      while (1);
      }

      else {
      Serial.print("Epoch received: ");
      Serial.println(epoch);
      rtc.setEpoch(epoch);
      Serial.println();
      }
}

void loop() {
  
  printTime();

  ambient();

  dhtsense();
  
  Serial.println("\n**********************************\n");
  delay(2000);

  /*if (hic >= 28) {
    IrSender.sendNEC(0x, 32); //turn AC on
    delay(1000);
    IrSender.sendNEC(0x, 32); //turn fan on
  }*/
}

void WiFiConnect() {
  // Check if the WiFi module works
  if (WiFi.status() == WL_NO_SHIELD) {
    // Wait until WiFi ready
    Serial.println("WiFi adapter not ready");
    while (true);

  }

      // Establish a WiFi connection
  while ( status != WL_CONNECTED) {

    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);

    // Wait 10 seconds for connection:
    delay(10000);

  }
}

void printWiFiStatus() {

  // Print the network SSID
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  
  // Print the IP address
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  
  // Print the received signal strength
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

void print2digits(int number) {

  if (number < 10) {
    Serial.print("0");
  }
  Serial.print(number);
}

void printTime()
{
  Serial.print("Time : ");
  print2digits(rtc.getHours() + GMT);
  Serial.print(":");
  
  print2digits(rtc.getMinutes());
  Serial.print(":");
  
  print2digits(rtc.getSeconds());
  Serial.println("\n");
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

  Serial.println("Humidity    : " + (String)h + " %");
  Serial.println("Temperature : " + (String)t + " °C");
  Serial.println("Heat Index  : " + (String)hic + " °C \n");
  delay(500);
}
