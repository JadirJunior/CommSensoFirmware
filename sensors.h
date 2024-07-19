#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>

#include <OneWire.h>
#include <DallasTemperature.h>


enum sensorsIds {
  TEMPERATURE = 1,
  UMIDITY = 2
};

struct ResponseMeasure {
  int idSensor;
  float value;
};

const int TEMPERATURE_PIN = 15;
const int UMIDITY_PIN = 33;

int analogDry = 4095; //Solo Seco
int analogWet = 1800; //Solo Molhado
int percDry = 0;
int percWet = 100;

OneWire oneWire(TEMPERATURE_PIN);

DallasTemperature sensor_temp(&oneWire);


void setupSensors() {
  pinMode(UMIDITY_PIN, OUTPUT);
  pinMode(TEMPERATURE_PIN, INPUT_PULLUP);
  sensor_temp.begin();
}


ResponseMeasure getUmidity() {
  Serial.println(analogRead(UMIDITY_PIN));
  int umidity = constrain(analogRead(UMIDITY_PIN), analogWet, analogDry);
  umidity = map(umidity, analogWet, analogDry, percWet, percDry);

  int sensorId = sensorsIds::UMIDITY;
  Serial.print("Umidade: ");
  Serial.print(umidity);
  Serial.println(" %");
  ResponseMeasure response = { sensorId, umidity };

  return response; 
}

ResponseMeasure getTemperature() {
  sensor_temp.requestTemperatures();  
  float temperature = sensor_temp.getTempCByIndex(0);

  if (temperature == DEVICE_DISCONNECTED_C) {
    Serial.println("Sensor de Temperatura desconectado");
    temperature = -127;
  }


  int sensorId = sensorsIds::TEMPERATURE;

  Serial.print("Temperatura: ");
  Serial.print(temperature);
  Serial.println(" ºC");

  ResponseMeasure response = { sensorId, temperature };

  return response;
}

#endif
