
#ifndef MEASURE_H
#define MEASURE_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <time.h>


#include "mqtt-manager.h"
#include "sensors.h"



extern bool Measuring = false;
extern unsigned int INTERVAL = 5000;
extern String CONTAINER = "TAMBOR 10"; 

//JSON
JsonDocument doc;

TaskHandle_t readMeasuresTask;

//Configuração do Servidor NTP
const long  gmtOffset_sec = -10800;
const int   daylightOffset_sec = 0;
const char* ntpServer = "south-america.pool.ntp.org";

void configLocalTime() {
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
}

//TIME FUNCTIONS
String localTime(){
  struct tm timeinfo;
  
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return "";
  }

  int year = timeinfo.tm_year + 1900;
  int mon = timeinfo.tm_mon + 1;
  int day = timeinfo.tm_mday;

  String date = String(year) + "-" + String(mon) + "-" + String(day) + " " + String(timeinfo.tm_hour) + ":" + String(timeinfo.tm_min) + ":" + String(timeinfo.tm_sec);

  return date;
}


void sendData(float value, int sensorId) {

  char output[128];
  doc["value"] = value;
  doc["sensorId"] = sensorId;
  doc["container"] = CONTAINER;
  doc["dtMeasure"] = localTime();

  doc.shrinkToFit();

  serializeJson(doc, output);

  publish(output);

}


void handleMeasure(void *pvParameters) {

  while (1) {
    Serial.println("Realizando a leitura dos dados...");
    ResponseMeasure response = getUmidity();
    //sendData(response.value, response.sensorId);
    response = getTemperature();
    sendData(response.value, response.idSensor);
    delay(INTERVAL);
  }
}




void registerTask() {
  setupSensors();
  xTaskCreatePinnedToCore(handleMeasure, "read-measures", 10000, NULL, 3, &readMeasuresTask, 0);
  vTaskSuspend(readMeasuresTask);
  
}

void startMeasures() {
  if (!Measuring) {
    vTaskResume(readMeasuresTask);
  }
}

void stopMeasures() {
  if (Measuring) {
    vTaskSuspend(readMeasuresTask);
  }
}


#endif