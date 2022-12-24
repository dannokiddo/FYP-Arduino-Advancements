// define Blynk connection
#define BLYNK_TEMPLATE_ID "TMPLsgwf8Dhk"
#define BLYNK_DEVICE_NAME "NIOT DRAFT"
#define BLYNK_AUTH_TOKEN "HfjOm1Ku95G8TjHITGlnsLMwEtVreCru"
#define BLYNK_PRINT Serial

// include libraries
#include "EmonLib.h"          //Energy Monitor
#include <WiFi.h>             //WiFi ESP32
#include <BlynkSimpleEsp32.h> //Blynk ESP32

// define identifiers
const char* ssid = "UniKL MIIT";
const char* pass = "";

BlynkTimer timer;

#define VCalibration 240//1.23
#define ICalibration 111.1

EnergyMonitor emon_AC;
EnergyMonitor emon_Fan;
EnergyMonitor emon_Bulb;

float kWh = 0;
unsigned long lastmillis = millis();

void setup() {

  WiFiConnect();
  printWiFiStatus();

  emon_AC.voltage(34, VCalibration, 1.7);
  emon_AC.current(35, ICalibration);

  Serial.begin(9600);
  timer.setInterval(5000L, AC_Mon);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

void loop() {
  Blynk.run();
  timer.run();
}

//########################################################################

void WiFiConnect() {
  // Connect to Wi-Fi
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid);//, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
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

void AC_Mon()
{
  emon_AC.calcVI(20, 2000);
  kWh = kWh + emon_AC.apparentPower * (millis() - lastmillis) / 3600000000.0;
  yield();
  Serial.print("Vrms: ");
  Serial.print(emon_AC.Vrms, 2);
  Serial.print("V");
 
  Serial.print("\tIrms: ");
  Serial.print(emon_AC.Irms, 4);
  Serial.print("A");
 
  Serial.print("\tPower: ");
  Serial.print(emon_AC.apparentPower, 4);
  Serial.print("W");
 
  Serial.print("\tkWh: ");
  Serial.print(kWh, 5);
  Serial.println("kWh");
 
  lastmillis = millis();
  
  Blynk.virtualWrite(V40, emon_AC.apparentPower);
  Blynk.virtualWrite(V41, kWh);
  Blynk.virtualWrite(V42, emon_AC.Vrms);
  Blynk.virtualWrite(V43, emon_AC.Irms);
}