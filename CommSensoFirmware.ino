#include <Arduino.h>

#include <Wire.h> //Biblioteca utilizada gerenciar a comunicação entre dispositicos através do protocolo I2C
#include <LiquidCrystal_I2C.h> //Biblioteca controlar display 16x2 através do I2C

#define col  16 //Define o número de colunas do display utilizado
#define lin   2 //Define o número de linhas do display utilizado
#define ende  0x3C //Define o endereço do display

#include "wifimanager.h"
#include "configure-spiffs.h"
#include "mqtt-manager.h"
#include "measure.h"
#include "sensors.h"

LiquidCrystal_I2C lcd(ende,16,2); //Cria o objeto lcd passando como parâmetros o endereço, o nº de colunas e o nº de linhas

void setup() {
  // Serial port for debugging purposes
  Serial.begin(9600);

  Wire.begin();

  lcd.init(); //Inicializa a comunicação com o display já conectado
  lcd.clear(); //Limpa a tela do display
  lcd.backlight(); //Aciona a luz de fundo do display
  
  initSPIFFS();

  //Fazer a leitura das credenciais já registradas (caso tenha)

  lcd.setCursor(0, 0);
  lcd.print("Lendo as credenciais"); 

  Serial.println("Fazendo a leitura das credenciais...");
  ssid = readFile(ssidPath);
  pass = readFile(passPath);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Credenciais lidas"); 

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


  lcd.setCursor(0, 0);
  lcd.print("Mqtt Conectado");

  client.loop();
}




