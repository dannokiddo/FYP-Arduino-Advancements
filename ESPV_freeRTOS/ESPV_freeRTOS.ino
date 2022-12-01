#include <WiFi.h>              
#include "time.h"

#define onboard_led 2

const char* ssid     = "Nazrin's Family";
const char* pass = "cheesecake6";

const char* ntpServer = "asia.pool.ntp.org";
const long  gmtOffset_sec = 28800;
const int   daylightOffset_sec = 28800;

void TaskBlink(void *pvParameters);
void TaskTime(void *pvParameters);
void TaskOutput(void *pvParameters);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  xTaskCreatePinnedToCore(
    TaskBlink,    // Task function
    "TaskBlink",  // Name of Task
    1024,         // Stack size
    NULL,         // Task Parameter
    2,            // Priotity of Task (3, 2, 1, 0)
    NULL,         // Task Handle; keep track of task
    0             // Task pinned to Core
  );

  xTaskCreatePinnedToCore(
    TaskTime,    // Task function
    "TaskTime",  // Name of Task
    1024,         // Stack size
    NULL,         // Task Parameter
    2,            // Priotity of Task (3, 2, 1, 0)
    NULL,         // Task Handle; keep track of task
    1             // Task pinned to Core
  );

  xTaskCreatePinnedToCore(
    TaskOutput,    // Task function
    "TaskOutput",  // Name of Task
    1024,         // Stack size
    NULL,         // Task Parameter
    1,            // Priotity of Task (3, 2, 1, 0)
    NULL,         // Task Handle; keep track of task
    1             // Task pinned to Core
  );

  WiFiConnect();
  printWiFiStatus();

  // Init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
}

void loop() {
  // put your main code here, to run repeatedly:
  
}

void TaskBlink(void *pvParameters) {
  pinMode(onboard_led, OUTPUT);

  for(;;) // runs on for loop infinitely
  {
    digitalWrite(onboard_led, HIGH);
    vTaskDelay(1000);
    digitalWrite(onboard_led, LOW);
    vTaskDelay(1000);
  }
}

void TaskTime(void *pvParameters) {
  

  for(;;) // runs on for loop infinitely
  {
    printTime();
    vTaskDelay(1000);
  }
}

void TaskOutput(void *pvParameters) {
  

  for(;;) // runs on for loop infinitely
  {
    Serial.println("eyyo");
    vTaskDelay(1000);
  }
}

void WiFiConnect() {
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

void printTime(){
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  /*Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
  Serial.print("Day of week: ");
  Serial.println(&timeinfo, "%A");
  Serial.print("Month: ");
  Serial.println(&timeinfo, "%B");
  Serial.print("Day of Month: ");
  Serial.println(&timeinfo, "%d");
  Serial.print("Year: ");
  Serial.println(&timeinfo, "%Y");
  Serial.print("Hour: ");
  Serial.println(&timeinfo, "%H");
  Serial.print("Hour (12 hour format): ");
  Serial.println(&timeinfo, "%I");
  Serial.print("Minute: ");
  Serial.println(&timeinfo, "%M");
  Serial.print("Second: ");
  Serial.println(&timeinfo, "%S");*/

  Serial.print("Time  : " );
  Serial.println(&timeinfo, "%H:%M:%S");

  /*Serial.println("Time variables");
  char timeHour[3];
  strftime(timeHour,3, "%H", &timeinfo);
  Serial.println(timeHour);*/
  char timeWeekDay[10];
  strftime(timeWeekDay,10, "%A", &timeinfo);
  Serial.println(timeWeekDay);
  Serial.println();
}