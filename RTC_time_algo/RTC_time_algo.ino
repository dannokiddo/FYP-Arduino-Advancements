#define BLYNK_TEMPLATE_ID "TMPLcs7W8DPD"
#define BLYNK_DEVICE_NAME "Virtual Pin"
#define BLYNK_AUTH_TOKEN "qjLKuRCsBbX105veJMmLc_b4Md5hnHOJ"

#include <SPI.h>
#include <WiFiNINA.h> 
#include <WiFiUdp.h>
#include <RTCZero.h>
#include <BlynkSimpleWiFiNINA.h>

// WiFi Credentials (edit as required)
char ssid[] = "Nazrin's Family";      // Wifi SSID
char pass[] = "cheesecake6";       // Wifi password

// Object for Real Time Clock
RTCZero rtc;

int status = WL_IDLE_STATUS;

const int GMT = +8; // Time zone constant

int btnV5;
int btn;

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
  btn = btnV5;
  printTime();
  delay(1000);

  //now = rtc.getHours() + GMT; //get hour now
  int now = rtc.getSeconds();

  do {
    /*now = rtc.getSeconds();
    BLYNK_WRITE(V5);

    Serial.println("while loop");
    delay(2000);

    Serial.println("btnV5:" + String(btnV5));
    Serial.println("btn:" + String(btn));

    if (btnV5 == 1 || btn == 1) break;
  } while (now >= 30 && now <=59);*/

  while (now >= 30 && btnV5 != 1 && btn != 1) {
    now = rtc.getSeconds();
    BLYNK_WRITE(V5);

    Serial.println("while loop");
    delay(2000);

    Serial.println("btnV5 :" + String(btnV5));
    Serial.println("btn   :" + String(btn));

    int btn = digitalRead(12);
    if (btnV5 == 1) break;
    else if (btn == 1) break;
  }*/
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

BLYNK_WRITE(V5)
{
  int btnV5 = param.asInt();
  Serial.println("blynk:" + String(btnV5));
}