#define BLYNK_TEMPLATE_ID "TMPLsgwf8Dhk"
#define BLYNK_DEVICE_NAME "NIOT DRAFT"
#define BLYNK_AUTH_TOKEN "HfjOm1Ku95G8TjHITGlnsLMwEtVreCru"
#define BLYNK_PRINT Serial

#include <Wire.h>
#include <INA3221.h>

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

const char* ssid = "matgyver";
const char* pass = "abcd1234";

#define SERIAL_SPEED     115200  // serial baud rate
#define PRINT_DEC_POINTS 3       // decimal points to print

// Set I2C address to 0x41 (A0 pin -> VCC)
INA3221 ina_0(INA3221_ADDR40_GND);
INA3221 ina_1(INA3221_ADDR41_VCC);

void current_measure_init() {
    ina_0.begin(&Wire);
    ina_0.reset();
    ina_0.setShuntRes(10, 10, 10);
    ina_1.begin(&Wire);
    ina_1.reset();
    ina_1.setShuntRes(10, 10, 10);
}

//#############################################################################
void setup() {

  WiFiConnect();
  printWiFiStatus();

  Serial.begin(SERIAL_SPEED);
  current_measure_init();

  while (!Serial) {
      delay(1);
  }
  // Set shunt resistors to 10 mOhm for all channels

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

//#############################################################################
void loop() {
  Blynk.run();

  Serial.printf(
      "A1: %3.0fma %1.1fV A2%3.0fma %1.1fV\r\n",
      ina_0.getCurrent(INA3221_CH1) * 1000, ina_0.getVoltage(INA3221_CH1),
      ina_0.getCurrent(INA3221_CH2) * 1000, ina_0.getVoltage(INA3221_CH2));
  Serial.printf(
      "B1: %3.0fma %1.1fV B2%3.0fma %1.1fV\r\n",
      ina_0.getCurrent(INA3221_CH3) * 1000, ina_0.getVoltage(INA3221_CH3),
      ina_1.getCurrent(INA3221_CH1) * 1000, ina_1.getVoltage(INA3221_CH1));
  Serial.printf(
      "C1: %3.0fma %1.1fV C2%3.0fma %1.1fV\r\n\n",
      ina_1.getCurrent(INA3221_CH2) * 1000, ina_1.getVoltage(INA3221_CH2),
      ina_1.getCurrent(INA3221_CH3) * 1000, ina_1.getVoltage(INA3221_CH3));

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