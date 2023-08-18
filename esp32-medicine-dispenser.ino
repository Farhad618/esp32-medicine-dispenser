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
const int LED_1 = 16;
const int LED_2 = 17;
const int LED_3 = 18;
const int LED_4 = 19;
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
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  pinMode(LED_3, OUTPUT);
  pinMode(LED_4, OUTPUT);
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
  // configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  /*if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return;
    }*/
  // while (!getLocalTime(&timeinfo)) {
  //   Serial.println("Trying to obtain time");
  //   delay(500);
  // }
  // Serial.println("Time obtained successfully");





  http.begin("http://192.168.43.223:3000/api/checkForAlarm");

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

  // updateLocalTime();
  // fetch start
  while (http.GET() != 200) {
    Serial.println("Tring to fetch data from API.");
    delay(500);
  }
  Serial.println("Data fetched successfully.");
  data = http.getString();

  http.end();
  // fetch end
  
  // Serial.println(&timeinfo);
  Serial.println(data);

  const char* string1 = data.c_str();
  if(strcmp(string1, "null") != 0){
    Serial.println("Alarm....");
    // doMedicationTimeAlarm(data);
    alarmOffButtonState = digitalRead(PULLUP_BUTTON_STOP_ALARM);

    while(alarmOffButtonState != 0){
      alarmOffButtonState = digitalRead(PULLUP_BUTTON_STOP_ALARM);
      
      int i = 0;
        
      // looping till the null character is encountered
      while(data[i] != '\0')
      {
        if(data[i] != ' '){
          printf(">%c<\n", data[i]);
          if(data[i]=='1'){
            digitalWrite(LED_1, HIGH);
          }
          if(data[i]=='2'){
            digitalWrite(LED_2, HIGH);
          }
          if(data[i]=='3'){
            digitalWrite(LED_3, HIGH);
          }
          if(data[i]=='4'){
            digitalWrite(LED_4, HIGH);
          }
        }
        i++;
      }
      digitalWrite(LED_BUILTIN, HIGH);
      beep(NOTE_A4, 500);
      delay(500);
      digitalWrite(LED_BUILTIN, LOW);
      digitalWrite(LED_1, LOW);
      digitalWrite(LED_2, LOW);
      digitalWrite(LED_3, LOW);
      digitalWrite(LED_4, LOW);
      beep(NOTE_A4, 500);
      delay(500);    
    }
    // Serial.println("Alarm off delay start....");
    // delay(60000);
    // Serial.println("Alarm off delay end....");
    


  }



  /*String timeFromApi = "";
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
  }*/




  

  delay(20000);

}
