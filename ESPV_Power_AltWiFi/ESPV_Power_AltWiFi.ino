// define Blynk connection
#define BLYNK_TEMPLATE_ID "TMPLsgwf8Dhk"
#define BLYNK_DEVICE_NAME "NIOT DRAFT"
#define BLYNK_AUTH_TOKEN "HfjOm1Ku95G8TjHITGlnsLMwEtVreCru"
#define BLYNK_PRINT Serial

// include libraries
#include "EmonLib.h"          // Energy Monitor
#include "ZMPT101B.h"         // Voltage Sensor
#include <WiFi.h>             // WiFi ESP32
#include <BlynkSimpleEsp32.h> // Blynk ESP32

// define identifiers
const char* ssid = "Nazrin's Family";//"UiTM_HOTSPOT";//"UniKL MIIT";
const char* pass = "cheesecake6";

BlynkTimer timer;

ZMPT101B voltageSensor(34);
#define ICalibration 111.1

float V;
float AC_W;
float Fan_W;
float Wattage;

float AC_Current;
float Fan_Current;

EnergyMonitor emon_AC;      //Currents Value
EnergyMonitor emon_Fan;

const unsigned long delay1s = 1000;
unsigned long rstdelay = 0;

void setup() {
  Serial.begin(9600);

  WiFiConnect();
  //printWiFiStatus();

  // ZMPT101B Set Up
  Serial.println("Calibrating... Ensure that no current flows through the sensor at this moment");
  delay(100);
  voltageSensor.calibrate();
  
  Serial.println("Done!");

  // EmonLib Set Up Current
  emon_AC.current(35, ICalibration);
  emon_Fan.current(39, ICalibration);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

void loop() {
  Blynk.run();

  AC_Current = emon_AC.calcIrms(5588);
  Fan_Current = emon_Fan.calcIrms(5588);

  unsigned long currentTime = millis();

  if (currentTime - rstdelay >= delay1s) {
    // Voltage
    ZMPT_Volt();
    Emon_Current();
    PowerWatt();

    // delay millis
    rstdelay = currentTime;
  }
  else{}

  
}

//########################################################################

void WiFiConnect() {
  // Connect to Wi-Fi
  int status = WL_IDLE_STATUS;
    Serial.println("\nConnecting");
    WiFi.begin(ssid, pass);
    while(status != WL_CONNECTED){
        delay(500);
        status = WiFi.status();
        Serial.println(status);
    }

    Serial.println("\nConnected to the WiFi network");
    Serial.print("Local ESP32 IP: ");
    Serial.println(WiFi.localIP());
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

void ZMPT_Volt()
{
  V = voltageSensor.getVoltageAC();

  if (V <= 200) {
    V = 0;
    Serial.println("\n\nVoltage : " + String(V));
    Blynk.virtualWrite(V41, V);
  }
  else {
    Serial.println("\n\nVoltage  : " + String(V));
    Blynk.virtualWrite(V41, V);
  }
}

void Emon_Current()
{
  Serial.print("\n\tAC_Irms : ");
  Serial.print(AC_Current, 4);
  Serial.print("A");

  Serial.print("  \tFan_Irms: ");
  Serial.print(Fan_Current, 4);
  Serial.print("A");

  Blynk.virtualWrite(V42, AC_Current);
  Blynk.virtualWrite(V43, Fan_Current);
}

void PowerWatt()
{
  AC_W = V * AC_Current;
  Fan_W = V * Fan_Current;
  Wattage = (V * (AC_W + Fan_W))/1000;

  Serial.print("\n\tPower AC  : " + String(AC_W));
  Serial.print("  \tPower Fan : " + String(Fan_W));
  Serial.println("  \tWattage   : " + String(Wattage));

  Blynk.virtualWrite(V40, Wattage);
}