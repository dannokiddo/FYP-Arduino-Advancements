// define Blynk connection
#define BLYNK_TEMPLATE_ID "TMPLsgwf8Dhk"
#define BLYNK_DEVICE_NAME "NIOT DRAFT"
#define BLYNK_AUTH_TOKEN  "HfjOm1Ku95G8TjHITGlnsLMwEtVreCru"

// include libraries
#include <DHT.h>                  //Temp & Humid
#include <IRremote.h>             //Infra-Red
#include <SPI.h>                  //WiFi
#include <WiFiNINA.h>             //WiFi Nano
#include <WiFiUdp.h>              //NTP Signal
#include <RTCZero.h>              //Real Time Clock
#include <BlynkSimpleWiFiNINA.h>  //Blynk 
#include <Fuzzy.h>                //Fuzzy lib

// define pins 
#define temt6000pin A0
#define recvpin     2
#define transpin    3
#define dhtpin      4   
#define radar      21
#define DHTTYPE DHT11

// define identifiers
int   status = WL_IDLE_STATUS;     // Wifi status
char  ssid[] = "UiTM_HOTSPOT";//"Nazrin's Family";  // Wifi SSID
char  pass[] = "";//"cheesecake6";      // Wifi password

Fuzzy  *fuzzy = new Fuzzy();
DHT   dht(dhtpin, DHTTYPE);      // DHT
float h, t, f;                   // DHT var; humid, temp, apparant temp
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

// Appliance
int ACTemp;
int tempSet;
int initspeed = 6;

uint32_t ONRawData[]={0x74070683, 0x0};   // AC ON
uint32_t OFFRawData[]={0x82070683, 0x0};  // AC OFF
uint32_t Temp16RawData[]={0x2010683, 0x0};
uint32_t Temp17RawData[]={0x12010683, 0x0};
uint32_t Temp18RawData[]={0x22010683, 0x0};
uint32_t Temp19RawData[]={0x32010683, 0x0};
uint32_t Temp20RawData[]={0x42010683, 0x0};
uint32_t Temp21RawData[]={0x52010683, 0x0};
uint32_t Temp22RawData[]={0x62010683, 0x0};
uint32_t Temp23RawData[]={0x72010683, 0x0};
uint32_t Temp24RawData[]={0x82010683, 0x0};
uint32_t Temp25RawData[]={0x92010683, 0x0};
uint32_t Temp26RawData[]={0xA2010683, 0x0};

// Blynk
int btnV10;   // AC
int btnV11;     // AC Timer
int btnV12;     // AC Up Temp
int btnV13;     // AC Down Temp
int btnV20;   // Light
int btnV21;     // Light Brightness
int btnV30;   // Fan
int btnV31;     // Fan Speed
int btnV32;     // Fan Timer
bool rstAC = false;
bool rstFan = false;
bool rstBulb = false;
bool stateAC = false;
bool stateFan = false;
bool stateBulb = false;

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

  FuzzySetup();

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

//==================================================================================
void loop() { 
  
  Blynk.run();

  //Serial Output
  SerialOutput();

  //Runs Scheduled Action
  //ScheduledAction_AC();
  ScheduledAction_Fan();
  FuzzySetTemp();

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
  h = dht.readHumidity();
  t = dht.readTemperature();
  f = dht.readTemperature(true);
  
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
    IrSender.sendNEC(0xEF00, 0x2, 3);         // OFF bulb
    IrSender.sendNEC(0x0, 0x1C, 0);           // OFF fan
    // OFF AC
    IrSender.sendPulseDistanceWidthFromArray
    (38, 8950, 4500, 600, 1600, 600, 500, &OFFRawData[0], 48, PROTOCOL_IS_LSB_FIRST, 0, 0);
    
  }
}

void Reset24Hr()
{
  //nowhr == rtc.getHours();
  nowmin == rtc.getMinutes();

  // (nowhr == 0) reset bool rstAC to false after 24hr
  if ((nowmin%5) == 4) {  // every 4 mins rst (4th min in 5 mins interval)
    rstAC = false;
    rstFan = false;
  }
}

void ScheduledAction_AC()
{
  //nowhr = rtc.getHours();   //get instant hour time
  nowmin = rtc.getMinutes();

  // (nowhr == 4 && rstAC == false)
  if ((nowmin%5) == 0 && rstAC == false)  // runs on mins%5. every 4 mins
  {
    // demo with bulb
    IrSender.sendNEC(0xEF00, 0x3, 1);//on bulb   
    IrSender.sendNEC(0xEF00, 0x7, 1);//white     

    // do AC on
    IrSender.sendPulseDistanceWidthFromArray
    (38, 8950, 4500, 600, 1650, 600, 550, &ONRawData[0], 48, PROTOCOL_IS_LSB_FIRST, 0, 0);
    
    FuzzySetTemp();                           // set AC temp

    stateAC = true;             //AC state on
    rstAC = true;               //rst state: done run once
    Serial.println("AC State" + stateAC);
  }
}

//void ScheduledAction_AC
void ScheduledAction_Fan()
{
  if ((nowmin%5) == 1 && rstFan == false)  // runs every 2nd min in 5 mins interval
  {
    IrSender.sendNEC(0x0, 0x1C, 1);  // Fan ON

    stateFan = true;
  }
}

void FuzzySetup()
{
  FuzzyInput *temp = new FuzzyInput(1);

  FuzzySet *low = new FuzzySet(18, 21, 21, 24);
  FuzzySet *mid = new FuzzySet(23, 26, 26, 29);
  FuzzySet *high = new FuzzySet(28, 31, 31, 34);

  temp -> addFuzzySet(low);
  temp -> addFuzzySet(mid);
  temp -> addFuzzySet(high);

  fuzzy -> addFuzzyInput(temp);

  FuzzyOutput *tempOut = new FuzzyOutput(1);

  FuzzySet *lowSet = new FuzzySet(16, 18, 18, 20);
  FuzzySet *midSet = new FuzzySet(19, 21, 21, 23);
  FuzzySet *highSet = new FuzzySet(22, 24, 24, 26);

  tempOut -> addFuzzySet(lowSet);
  tempOut -> addFuzzySet(midSet);
  tempOut -> addFuzzySet(highSet);

  fuzzy->addFuzzyOutput(tempOut);

  // Rule 01 - if low, then set high
  FuzzyRuleAntecedent *ifTempLow = new FuzzyRuleAntecedent();
  ifTempLow -> joinSingle(low);

  FuzzyRuleConsequent *thenSetHigh = new FuzzyRuleConsequent();
  thenSetHigh -> addOutput(highSet);

  FuzzyRule *fuzzyRule01 = new FuzzyRule(1, ifTempLow, thenSetHigh);
  fuzzy -> addFuzzyRule(fuzzyRule01);

  // Rule 02 - if mid, then set mid
  FuzzyRuleAntecedent *ifTempMid = new FuzzyRuleAntecedent();
  ifTempLow -> joinSingle(mid);

  FuzzyRuleConsequent *thenSetMid = new FuzzyRuleConsequent();
  thenSetHigh -> addOutput(midSet);

  FuzzyRule *fuzzyRule02 = new FuzzyRule(2, ifTempMid, thenSetMid);
  fuzzy -> addFuzzyRule(fuzzyRule02);

  // Rule 03 - if high, then set low
  FuzzyRuleAntecedent *ifTempHigh = new FuzzyRuleAntecedent();
  ifTempLow -> joinSingle(high);

  FuzzyRuleConsequent *thenSetLow = new FuzzyRuleConsequent();
  thenSetHigh -> addOutput(lowSet);

  FuzzyRule *fuzzyRule03 = new FuzzyRule(3, ifTempHigh, thenSetLow);
  fuzzy -> addFuzzyRule(fuzzyRule03);
}

void FuzzySetTemp()
{
  //int temp = t;
  fuzzy -> setInput(1, t);

  fuzzy -> fuzzify();

  tempSet = fuzzy -> defuzzify(1);
  Serial.println("Fuzzy Temp: " + String(tempSet));

  if (stateAC) {    // if stateAC is true/ON then tempSet is sent
    switch (tempSet) {
      case 26:
        IrSender.sendPulseDistanceWidthFromArray(38, 8950, 4500, 600, 1650, 600, 550, &Temp26RawData[0], 48, PROTOCOL_IS_LSB_FIRST, 0, 0);
        break;
      case 25:
        IrSender.sendPulseDistanceWidthFromArray(38, 9000, 4450, 600, 1650, 600, 550, &Temp25RawData[0], 48, PROTOCOL_IS_LSB_FIRST, 0, 0);
        break;
      case 24:
        IrSender.sendPulseDistanceWidthFromArray(38, 8950, 4500, 550, 1700, 550, 550, &Temp24RawData[0], 48, PROTOCOL_IS_LSB_FIRST, 0, 0);
        break;
      case 23:
        IrSender.sendPulseDistanceWidthFromArray(38, 8950, 4500, 600, 1650, 600, 550, &Temp23RawData[0], 48, PROTOCOL_IS_LSB_FIRST, 0, 0);
        break;
      case 22:
        IrSender.sendPulseDistanceWidthFromArray(38, 8900, 4500, 600, 1650, 600, 550, &Temp22RawData[0], 48, PROTOCOL_IS_LSB_FIRST, 0, 0);
        break;        
      case 21:
        IrSender.sendPulseDistanceWidthFromArray(38, 8900, 4550, 550, 1700, 550, 550, &Temp21RawData[0], 48, PROTOCOL_IS_LSB_FIRST, 0, 0);
        break;
      case 20:
        IrSender.sendPulseDistanceWidthFromArray(38, 8950, 4450, 600, 1650, 600, 550, &Temp20RawData[0], 48, PROTOCOL_IS_LSB_FIRST, 0, 0);
        break;
      case 19:
        IrSender.sendPulseDistanceWidthFromArray(38, 8950, 4500, 600, 1650, 600, 550, &Temp19RawData[0], 48, PROTOCOL_IS_LSB_FIRST, 0, 0);
        break;
      case 18:
        IrSender.sendPulseDistanceWidthFromArray(38, 8950, 4500, 600, 1650, 600, 550, &Temp18RawData[0], 48, PROTOCOL_IS_LSB_FIRST, 0, 0);
        break;
      case 17:
        IrSender.sendPulseDistanceWidthFromArray(38, 9000, 4450, 600, 1650, 600, 500, &Temp17RawData[0], 48, PROTOCOL_IS_LSB_FIRST, 0, 0);
        break;
      case 16:
        IrSender.sendPulseDistanceWidthFromArray(38, 9000, 4450, 600, 1600, 600, 500, &Temp16RawData[0], 48, PROTOCOL_IS_LSB_FIRST, 0, 0);
        break;
      default:
        Serial.println("AC temp control Err");
        break;
    }
  }
}

BLYNK_WRITE(V10)    // AC
{
  btnV10 = param.asInt();

  if (btnV10 == 0 && stateAC == true) {
    // OFF AC
    IrSender.sendPulseDistanceWidthFromArray
    (38, 8950, 4500, 600, 1600, 600, 500, &OFFRawData[0], 48, PROTOCOL_IS_LSB_FIRST, 0, 0);
    stateAC = false;                 //AC state off
  }

  else if (btnV10 == 1) {
    IrSender.sendPulseDistanceWidthFromArray
    (38, 8950, 4500, 600, 1650, 600, 550, &ONRawData[0], 48, PROTOCOL_IS_LSB_FIRST, 0, 0);
    stateAC = true;                   //AC state on

    // If Absent 
    AutoShutOff();
  }
}

BLYNK_WRITE(V12)    // AC Up Temp
{
  btnV12 = param.asInt();

  if (btnV12 == 1 && stateAC == true) {
    uint32_t UPRawData[]={0x92030683, 0x0};
    IrSender.sendPulseDistanceWidthFromArray(38, 8950, 4500, 600, 1650, 600, 550, &UPRawData[0], 48, PROTOCOL_IS_LSB_FIRST, 0, 0);
    delay(5000);
  }
}

BLYNK_WRITE(V13)    // AC Down Temp
{
  btnV13 = param.asInt();

  if (btnV13 == 1 && stateAC == true) {
    uint32_t DOWNRawData[]={0x82030683, 0x0};
    IrSender.sendPulseDistanceWidthFromArray(38, 9000, 4450, 600, 1600, 600, 500, &DOWNRawData[0], 48, PROTOCOL_IS_LSB_FIRST, 0, 0);
    delay(5000);
  }
}

BLYNK_WRITE(V20)    // Light
{
  btnV20 = param.asInt();

  if (btnV20 == 0 && stateAC == true) {
    IrSender.sendNEC(0xEF00, 0x2, 3); //off bulb
    stateBulb = false;                //AC state off
  }

  else if (btnV20 == 1) {
    IrSender.sendNEC(0xEF00, 0x3, 3); //do bulb on
    IrSender.sendNEC(0xEF00, 0x7, 3); //do bulb white
    stateBulb = true;                 //AC state on

    // If Absent = Shut Off
    AutoShutOff();
  }
}

BLYNK_WRITE(V21)    // Light Brightness
{
  btnV21 = param.asInt();

  if (stateBulb == true) {  // if Bulb is ON do...
    if (btnV21 == 1) {
      IrSender.sendNEC(0xEF00, 0x0, 2); // Dim Up
    }

    else if (btnV21 == 2) {
      IrSender.sendNEC(0xEF00, 0x0, 4); // Dim Up+
    }

    else if (btnV21 == -1) {
      IrSender.sendNEC(0xEF00, 0x1, 2); // Dim Down
    }

    else if (btnV21 == -2) {
      IrSender.sendNEC(0xEF00, 0x1, 4); // Dim Down-
    }
  }
}

BLYNK_WRITE(V30)    // Fan
{
  btnV30 = param.asInt();

  if (btnV30 == 0 && stateFan == true) {
    IrSender.sendNEC(0xEF00, 0x2, 3); // do Fan off
    stateFan = false;                  // Fan state off
  }

  else if (btnV30 == 1) {
    IrSender.sendNEC(0xEF00, 0x3, 3); // do Fan on
    stateFan = true;                  // Fan state on

    // If Absent 
    AutoShutOff();
  }
}

BLYNK_WRITE(V31)    // Fan Speed
{
  btnV31 = param.asInt();

  if (stateFan == true) {  // if Fan is ON do...
    if (btnV31 == 1) {
      IrSender.sendNEC(0x0, 0x16, 9); // Speed 1 //test
    }

    else if (btnV31 == 2) {
      IrSender.sendNEC(0x10, 0x16, 2); // Speed 2
    }

    else if (btnV31 == 3) {
      IrSender.sendNEC(0x10, 0x1A, 2); // Speed 3
    }
  }
}

BLYNK_WRITE(V32)    // Fan Timer
{
  btnV32 = param.asInt();

  if (stateFan == true) {  // if Fan is ON do...
    if (btnV32 == 1) {
      IrSender.sendNEC(0x0, 0x40, 4); // 1 Hr //test if its viable if not have tu dupe code
    }

    else if (btnV32 == 2) {
      IrSender.sendNEC(0x0, 0x40, 5); // 2 Hr
    }
  }
}