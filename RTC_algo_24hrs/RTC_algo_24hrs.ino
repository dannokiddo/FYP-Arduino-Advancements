#define BLYNK_TEMPLATE_ID "TMPLcs7W8DPD"
#define BLYNK_DEVICE_NAME "Virtual Pin"
#define BLYNK_AUTH_TOKEN "qjLKuRCsBbX105veJMmLc_b4Md5hnHOJ"

#include <SPI.h>
#include <WiFiNINA.h> 
#include <WiFiUdp.h>
#include <RTCZero.h>
#include <BlynkSimpleWiFiNINA.h>

// WiFi Credentials (edit as required)
char ssid[] = "Nazrin's Family";   // Wifi SSID
char pass[] = "cheesecake6";       // Wifi password
int status = WL_IDLE_STATUS;       // WiFi stats

// Object for Real Time Clock
RTCZero rtc;
int now;
const int GMT = +8;       // Time zone constant
bool reset24hr = false;   // Run once per 24hr

// Blynk
int btnV5;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  // Print connection status
  WiFiConnect();
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

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}
//**********************************************************************************
void loop() {
  Blynk.run();
  // put your main code here, to run repeatedly:
  printTime();
  delay(1000);

  //now = rtc.getHours() + GMT; //get hour now
  now = rtc.getSeconds();

  while (now >= 30 && now <= 59 && reset24hr == false) {
    reset24hr = true;
    Serial.println("run once");
    
    Serial.println("loop");
    delay(1000);
    Blynk.virtualWrite(V6, HIGH);
  }
}

//##################################################################################
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

  print2digits(rtc.getHours() + GMT);
  Serial.print(":");
  
  print2digits(rtc.getMinutes());
  Serial.print(":");
  
  print2digits(rtc.getSeconds());
  Serial.println();
}

void dailyreset()
{
  if (now == 0) { 
    reset24hr = false;
    Serial.println("reset taken effect.");
  }
}

BLYNK_WRITE(V5)
{
  btnV5 = param.asInt();
  Serial.println("blynk:" + String(btnV5));

  if (btnV5 == 1) {
    Blynk.virtualWrite(V6, LOW);
  }
}