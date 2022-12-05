#define BLYNK_TEMPLATE_ID "TMPLsgwf8Dhk"
#define BLYNK_DEVICE_NAME "NIOT DRAFT"
#define BLYNK_AUTH_TOKEN "HfjOm1Ku95G8TjHITGlnsLMwEtVreCru"
#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

#include <Wire.h>
#include <Beastdevices_INA3221.h>

const char* ssid = "matgyver";
const char* pass = "abcd1234";

#define SERIAL_SPEED      115200    // serial baud rate
#define PRINT_DEC_POINTS  3         // decimal points to print

// Set I2C address to 0x41 (A0 pin -> VCC)
Beastdevices_INA3221 ina3221(INA3221_ADDR41_VCC);

//#############################################################################
void setup() {

  WiFiConnect();
  printWiFiStatus();

  Serial.begin(SERIAL_SPEED);

  while (!Serial) {
      delay(1);
  }

  ina3221.begin();
  ina3221.reset();

  // Set shunt resistors to 10 mOhm for all channels
  ina3221.setShuntRes(10, 10, 10);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

//#############################################################################
void loop() {
  Blynk.run();

  float current[3];
  float voltage[3];

  current[0] = ina3221.getCurrent(INA3221_CH1);
  voltage[0] = ina3221.getVoltage(INA3221_CH1);

  current[1] = ina3221.getCurrent(INA3221_CH2);
  voltage[1] = ina3221.getVoltage(INA3221_CH2);

  current[2] = ina3221.getCurrent(INA3221_CH3);
  voltage[2] = ina3221.getVoltage(INA3221_CH3);

  Serial.print("Channel 1: ");
  Serial.print(current[0], PRINT_DEC_POINTS);
  Serial.print("A, ");
  Serial.print(voltage[0], PRINT_DEC_POINTS);
  Serial.println("V/n");

  Serial.print("Channel 2: ");
  Serial.print(current[1], PRINT_DEC_POINTS);
  Serial.print("A, ");
  Serial.print(voltage[1], PRINT_DEC_POINTS);
  Serial.println("V/n");

  Serial.print("Channel 3: ");
  Serial.print(current[2], PRINT_DEC_POINTS);
  Serial.print("A, ");
  Serial.print(voltage[2], PRINT_DEC_POINTS);
  Serial.println("V/n");

  Serial.println("----------------------------------------");
  delay(1000);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void WiFiConnect()
{
  // Connect to Wi-Fi
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
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