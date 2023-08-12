#include <HTTPClient.h>
#include <WiFi.h>
#include <stdio.h>
#include <string.h>
#include "time.h"
#include "pitches.h"


const char* ssid       = "esp32";
const char* password   = "amibolbona";

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 16250;
const int   daylightOffset_sec = 3600;

HTTPClient http;
struct tm timeinfo;
String data;
int loopCounter = 0;
int alarmOffButtonState = 1;
char timeHMForAlarmOff[6];

const int LED_BUILTIN = 2;
const int buzzerPin = 23;
const int PULLUP_BUTTON_STOP_ALARM = 22;

void updateLocalTime() {
  while (!getLocalTime(&timeinfo)) {
    Serial.println("Trying to obtain time");
    delay(500);
  }
  Serial.println("Time obtained successfully");
}

void beep(int note, int duration){
  tone(buzzerPin, note, duration);
  delay(duration);
}

void doMedicationTimeAlarm(){
//  char presentTimeHM[6];
//  strftime(presentTimeHM, 6, "%H:%M", &timeinfo);
    alarmOffButtonState = digitalRead(PULLUP_BUTTON_STOP_ALARM);

  while(alarmOffButtonState != 0){
    alarmOffButtonState = digitalRead(PULLUP_BUTTON_STOP_ALARM);
    digitalWrite(LED_BUILTIN, HIGH);
    beep(NOTE_A4, 500);
    delay(500);
    digitalWrite(LED_BUILTIN, LOW);
    beep(NOTE_A4, 500);
    delay(500);    
  }
  Serial.println("Alarm off delay start....");
  delay(60000);
  Serial.println("Alarm off delay end....");
}



void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(PULLUP_BUTTON_STOP_ALARM, INPUT_PULLUP);
  
   WiFi.begin(ssid, password);
//  WiFi.begin("Wokwi-GUEST", "", 6);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");


  //init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  /*if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return;
    }*/
  // while (!getLocalTime(&timeinfo)) {
  //   Serial.println("Trying to obtain time");
  //   delay(500);
  // }
  // Serial.println("Time obtained successfully");





  http.begin("https://datasavefromapi.abu-taha-md-far.repl.co/all");

  /*if (http.GET() == 200) {
    data = http.getString();
    } else {
    Serial.println("Error fetching data.");
    }*/

 /* while (http.GET() != 200) {
    Serial.println("Tring to fetch data from API.");
    delay(500);
  }
  Serial.println("Data fetched successfully.");
  data = http.getString();

  http.end();*/
}

void loop() {
  Serial.println(++loopCounter);

  updateLocalTime();
  // fetch start
  while (http.GET() != 200) {
    Serial.println("Tring to fetch data from API.");
    delay(500);
  }
  Serial.println("Data fetched successfully.");
  data = http.getString();

  http.end();
  // fetch end
  
  Serial.println(&timeinfo);
  Serial.println(data);

  String timeFromApi = "";
  int i = 0;
  while (1) {

    if (data[i] != ',' && data[i] != '\0') {
      timeFromApi += data[i];

    } else {
      // Serial.println(timeFromApi);
      char timeHM[6];
      strftime(timeHM, 6, "%H:%M", &timeinfo);
      const char* string1 = timeFromApi.c_str();
      if (strcmp(string1, timeHM) == 0) {
        strftime(timeHMForAlarmOff, 6, "%H:%M", &timeinfo);
        Serial.println("Alarm....");
        doMedicationTimeAlarm();
      }

      timeFromApi = "";
    }
    if (data[i] == '\0') {
      break;
    }
    i++;
  }




  

  delay(1000);

}
