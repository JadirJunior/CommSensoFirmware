
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

    ResponseMeasure umidity = getMeasure(umid, 100, sensorsIds::UMIDITY);
    Serial.print("Umidade: ");
    Serial.print(umidity.value);
    Serial.println(" %");

    sendData(umidity.value, umidity.idSensor);

    delay(1000);

    ResponseMeasure temperature = getMeasure(temp, 100, sensorsIds::TEMPERATURE);
    Serial.print("Temperatura: ");
    Serial.print(temperature.value);
    Serial.println(" ºC");

    sendData(temperature.value, temperature.idSensor);

    delay(1000);

    ResponseMeasure nitrogen = getMeasure(nitro, 1, sensorsIds::NITROGEN);
    Serial.print("Nitrogênio: ");
    Serial.print(nitrogen.value);
    Serial.println("  mg/kg");

    sendData(nitrogen.value, nitrogen.idSensor);


    delay(1000);

    ResponseMeasure phosp = getMeasure(phos, 1, sensorsIds::PHOSPHORUS);
    Serial.print("Fósforo: ");
    Serial.print(phosp.value);
    Serial.println("  mg/kg");
    sendData(phosp.value, phosp.idSensor);

    delay(1000);


    ResponseMeasure phValue = getMeasure(Ph, 10, sensorsIds::PH);
    Serial.print("Ph: ");
    Serial.println(phValue.value);
    sendData(phValue.value, phValue.idSensor);

    delay(1000);


    ResponseMeasure condValue = getMeasure(cond, 1, sensorsIds::CONDUCTIVITY);
    Serial.print("Condutividade: ");
    Serial.print(condValue.value);
    Serial.println(" uS");
    sendData(condValue.value, condValue.idSensor);

    delay(1000);


    ResponseMeasure potassio = getMeasure(phota, 1, sensorsIds::POTASSIUM);
    Serial.print("Potássio: ");
    Serial.print(potassio.value);
    Serial.println(" mg/kg");
    sendData(potassio.value, potassio.idSensor);


    delay(INTERVAL);
  }
}




void registerTask() {
  setupSensors();
  xTaskCreatePinnedToCore(handleMeasure, "read-measures", 10000, NULL, 5, &readMeasuresTask, 1);
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
    Measuring = false;
  }
}


#endif