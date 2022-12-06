// define Blynk connection
#define BLYNK_TEMPLATE_ID "TMPLsgwf8Dhk"
#define BLYNK_DEVICE_NAME "NIOT DRAFT"
#define BLYNK_AUTH_TOKEN  "HfjOm1Ku95G8TjHITGlnsLMwEtVreCru"

// include libraries
#include <DHT.h>                  //temp & humid
#include <IRremote.h>
#include <SPI.h>                  //wifi **Week6
#include <WiFiNINA.h> 
#include <WiFiUdp.h>              //ntp signal
#include <RTCZero.h>              //real time clock 
#include <BlynkSimpleWiFiNINA.h>  //Blynk **

// define pins 
#define temt6000pin A0
#define recvpin     2
#define transpin    3
#define dhtpin      4   
#define radar      21
#define DHTTYPE DHT11

// define identifiers
int   status = WL_IDLE_STATUS;     // Wifi status
char  ssid[] = "Nazrin's Family";  // Wifi SSID
char  pass[] = "cheesecake6";      // Wifi password

DHT   dht(dhtpin, DHTTYPE);      // DHT
int   motion;       // read      // Microwave Radar
int   presence;     // object (human)

RTCZero rtc;                     // Time
const int GMT = +8;              // Time zone constant
int   nowhr;        // hr instance
int   nowmin;       // min instance
int   runTime;      // appliance run time
int   periodAbsent; // absent(t)
bool  rstAbsent;    // absent run once
bool  periodrst;    // rst status

// Blynk
int btnV5;
bool rstAC = false;
bool rstFan = false;
bool stateAC = false;
bool stateFan = false;

//==================================================================================
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
  pinMode(recvpin, INPUT);
  pinMode(radar, INPUT);

  // NTP Fetch Service~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
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
    }//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

//==================================================================================
void loop() { 
  
  Blynk.run();

  //Serial Output
  SerialOutput();

  //Runs Scheduled Action
  ScheduledAction_Light();

  AutoShutOff();  // Shut OFF when Absent
  Reset24Hr();    // reset bool rstAC to false after 24hr
}

//##################################################################################

void SerialOutput() { // Serial Monitor Output
  printTime();
  ambient();
  dhtsense();
  HumanPresence();
  Serial.println("\n**********************************\n");
  delay(2000);
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

void print2digits(int number) { // Time Formatter

  if (number < 10) {
    Serial.print("0");
  }
  Serial.print(number);
}

void printTime() {  // Time
  Serial.print("Time  : ");
  print2digits(rtc.getHours() + GMT);
  Serial.print(":");
  
  print2digits(rtc.getMinutes());
  Serial.print(":");
  
  print2digits(rtc.getSeconds());
  Serial.println("\n");

  String time = String(rtc.getHours() + GMT) + ":" + String(rtc.getMinutes());
  Blynk.virtualWrite(V3, time);
}

void ambient() {  // Ambient Lighting
  // ambient light
  int light_val = analogRead(temt6000pin);
  float lux = light_val * 0.9765625;  // 1000/1024
  Serial.println("Luminance   : " + (String)lux + " lux\n");
  delay(500);

  Blynk.virtualWrite(V0, lux);
}

void dhtsense() { // Temperature & Humidity
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
  //Serial.print("    ");
  Serial.println("Temperature : " + (String)t + " °C");
  //Serial.println("Heat Index  : " + (String)hic + " °C \n");
  delay(500);

  Blynk.virtualWrite(V1, h);  //Blynk Input
  Blynk.virtualWrite(V2, t);
}

void HumanPresence()  // Human Detection
{  
  motion = digitalRead(radar);
  delay(1000);

  if (motion == HIGH) {
    String detect = "Presence    : Detected";
    Serial.println("\n" + detect);

    presence = 1;
    rstAbsent = false;    // reset Absent status
    Blynk.virtualWrite(V4, detect);
  }
  else {
    String nodetect = "Presence    : Absent";
    Serial.println("\n" + nodetect);

    presence = 0;
    Blynk.virtualWrite(V4, nodetect);
    
    if (presence == 0 && rstAbsent == false) {  
      periodAbsent = millis();    // start timer when absent
    }
  }
}

void AutoShutOff()  // Shut Off 
{  
  if (periodAbsent - 120000 >= 0) {     // after 2 mins Shut OFF
    // OFF appliances
    IrSender.sendNEC(0xEF00, 0x2, 3);   // OFF bulb
  }
}

void Reset24Hr()
{
  //nowhr == rtc.getHours();
  nowmin == rtc.getMinutes();

  // (nowhr == 0) reset bool rstAC to false after 24hr
  if ((nowmin%10) == 0) {  // every 10 mins rst
    rstAC = false;
  }
}

void ScheduledAction_Light()
{
  //nowhr = rtc.getHours();   //get instant hour time
  nowmin = rtc.getMinutes();

  // (nowhr == 4 && rstAC == false)
  if ((nowmin%5) == 0 && rstAC == false)  // every 5 mins
  {
    // demo with bulb
    IrSender.sendNEC(0xEF00, 0x3, 1);//on bulb   // do AC on
    IrSender.sendNEC(0xEF00, 0x7, 1);//white     // set AC tmr

    stateAC = true;             //AC state on
    rstAC = true;               //rst state: done run once
  }
}

BLYNK_WRITE(V5)
{
  btnV5 = param.asInt();

  if (btnV5 == 0 && stateAC == true) {
    IrSender.sendNEC(0xEF00, 0x2, 3);//off bulb   //do AC off
    stateAC = false;                 //AC state off
  }

  else if (btnV5 == 1) {
    IrSender.sendNEC(0xEF00, 0x3, 3); //do AC on
    stateAC = true;                   //AC state on
  }
}
