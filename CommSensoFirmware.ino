#include <Arduino.h>

#include "wifimanager.h"
#include "configure-spiffs.h"
#include "mqtt-manager.h"
#include "measure.h"
#include "sensors.h"

void setup() {
  // Serial port for debugging purposes
  Serial.begin(9600);

  initSPIFFS();

  //Fazer a leitura das credenciais já registradas (caso tenha)
  Serial.println("Fazendo a leitura das credenciais...");
  ssid = readFile(ssidPath);
  pass = readFile(passPath);

  // Serial.println(ssid);
  // Serial.println(pass);

  if(initWiFi()) {  
    startWebServer();
  }
  else {
    startCredentialsServer();
  }

  while (!isConnected());

  configLocalTime();
  
  // //Depois de configurado o WiFi, o MQTT será devidamente configurado.
  configureMqttServer();

  registerTask();

}

void loop() {
  
  if (!isConnected()) {
    Serial.println("Conexão perdida, reiniciando o ESP...");
    stopMeasures();
    ESP.restart();
  }

  if (!mqttConnected()) {
    stopMeasures();
    delay(500);
    reconnect();
  }
  
  client.loop();
}




