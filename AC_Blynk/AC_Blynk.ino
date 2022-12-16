// define Blynk connection
#define BLYNK_TEMPLATE_ID "TMPLsgwf8Dhk"
#define BLYNK_DEVICE_NAME "NIOT DRAFT"
#define BLYNK_AUTH_TOKEN  "HfjOm1Ku95G8TjHITGlnsLMwEtVreCru"
#define BLYNK_PRINT Serial

// include libraries
#include <WiFiNINA.h>             //WiFi Nano
#include <IRremote.h>             //Infra-Red
#include <BlynkSimpleWiFiNINA.h>  //Blynk 

// define pins
#define transpin    3

// Blynk
int btnV10;   // AC
int btnV11;     // AC Timer
int btnV12;     // AC Up Temp
int btnV13;     // AC Down Temp
bool rstAC = false;
bool rstFan = false;
bool rstBulb = false;
bool stateAC = false;
bool stateFan = false;
bool stateBulb = false;

// define identifiers
int   status = WL_IDLE_STATUS;     // Wifi status
char  ssid[] = "UiTM WiFi Guest";//"Nazrin's Family";  // Wifi SSID
char  pass[] = "";//"cheesecake6";      // Wifi password

void setup() {
  // put your setup code here, to run once:
  // Begin Serial Monitor
  Serial.begin(9600);

  WiFiConnect();
  printWiFiStatus();

  IrSender.begin(transpin);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);  

}

void loop() {
  // put your main code here, to run repeatedly:
  Blynk.run();
}

void WiFiConnect() {  // Connect WiFi
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

void printWiFiStatus() {  // WiFi Status

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

BLYNK_WRITE(V10)    // AC
{
  btnV10 = param.asInt();

  if (btnV10 == 0 && stateAC == true) {
    uint32_t OFFRawData[]={0x82070683, 0x0};
    IrSender.sendPulseDistanceWidthFromArray(38, 8950, 4500, 600, 1600, 600, 500, &OFFRawData[0], 48, PROTOCOL_IS_LSB_FIRST, 0, 0);
  
    stateAC = false;                 //AC state off
  }

  else if (btnV10 == 1) {
    uint32_t tRawData[]={0x74070683, 0x0};
    IrSender.sendPulseDistanceWidthFromArray(38, 8950, 4500, 600, 1650, 600, 550, &tRawData[0], 48, PROTOCOL_IS_LSB_FIRST, 0, 0);
  
    stateAC = true;                   //AC state on

    // If Absent 
    //AutoShutOff();
  }
}

BLYNK_WRITE(V12)
{
  btnV12 = param.asInt();

  if (btnV12 == 1 && stateAC == true) {
    uint32_t UPRawData[]={0x92030683, 0x0};
    IrSender.sendPulseDistanceWidthFromArray(38, 8950, 4500, 600, 1650, 600, 550, &UPRawData[0], 48, PROTOCOL_IS_LSB_FIRST, 0, 0);
    delay(5000);
  }
}

BLYNK_WRITE(V13)
{
  btnV13 = param.asInt();

  if (btnV13 == 1 && stateAC == true) {
    uint32_t DOWNRawData[]={0x82030683, 0x0};
    IrSender.sendPulseDistanceWidthFromArray(38, 9000, 4450, 600, 1600, 600, 500, &DOWNRawData[0], 48, PROTOCOL_IS_LSB_FIRST, 0, 0);
    delay(5000);
  }
}