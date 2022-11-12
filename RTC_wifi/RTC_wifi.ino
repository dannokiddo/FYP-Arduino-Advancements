/*
  Arduino Nano 33 IoT WiFi RTC Demo
  nano-33-iot-wifi-rtc.ino
  Demonstrates use of Real Time Clock
  Gets Linux epoch from network NTP service and determines current time
  Results on serial monitor

  DroneBot Workshop 2021
  https://dronebotworkshop.com
  
  Based upon Arduino example by Arturo Guadalupi
*/

// Include required libraries
#include <SPI.h>
#include <WiFiNINA.h> 
#include <WiFiUdp.h>
#include <RTCZero.h>

// WiFi Credentials (edit as required)
char ssid[] = "Nazrin's Family";      // Wifi SSID
char pass[] = "cheesecake6";       // Wifi password
int keyIndex = 0;                // Network key Index number (needed only for WEP)

// Object for Real Time Clock
RTCZero rtc;

int status = WL_IDLE_STATUS;

// Time zone constant - change as required for your location
const int GMT = +8; 

void printTime()
{

  print2digits(rtc.getHours() + GMT);
  Serial.print(":");
  
  print2digits(rtc.getMinutes());
  Serial.print(":");
  
  print2digits(rtc.getSeconds());
  Serial.println();
}

void printDate()
{

  Serial.print(rtc.getDay());
  Serial.print("/");
  
  Serial.print(rtc.getMonth());
  Serial.print("/");
  
  Serial.print(rtc.getYear());
  Serial.print(" ");
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


void setup() {

  // Start Serial port
  Serial.begin(115200);

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

  // Print connection status
  printWiFiStatus();
  
  // Start Real Time Clock
  rtc.begin();
  
  // Variable to represent epoch
  unsigned long epoch;
 
 // Variable for number of tries to NTP service
  int numberOfTries = 0, maxTries = 6;

 // Get epoch
  do {
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

  printDate();
  printTime();
  Serial.println();

  delay(1000);
}