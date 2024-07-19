
#ifndef WIFIMANAGER_H
#define WIFIMANAGER_H
#include <Arduino.h>

#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>

#include "configure-spiffs.h"
#include "measure.h"


AsyncWebServer server(80);

const char* PARAM_INPUT_1 = "ssid";
const char* PARAM_INPUT_2 = "pass";

const char* PARAM_INTERVAL = "interval";
const char* PARAM_SENSOR = "sensor";
const char* PARAM_CONTAINER = "container";

String MeasureState;

//Variables to save values from HTML form
String ssid;
String pass;

// Arquivos para salvar credenciais de WiFi
const char* ssidPath = "/ssid.txt";
const char* passPath = "/pass.txt";


// Variáveis para timeout de conexão com WiFi
unsigned long previousMillis = 0;
const long interval = 10000; 


const char PAGE_MAIN[] PROGMEM = R"=====(

<!DOCTYPE html>
<html>

<head>
    <title>ESP Wi-Fi Manager</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link rel="icon" href="data:,">
    <style>
      html {
          font-family: Arial, Helvetica, sans-serif;
          display: inline-block;
          text-align: center;
      }

      h1 {
          font-size: 1.8rem;
          color: white;
      }

      p {
          font-size: 1.4rem;
      }

      .topnav {
          overflow: hidden;
          background-color: #0A1128;
      }

      body {
          margin: 0;
      }

      .content {
          padding: 5%;
      }

      .card-grid {
          max-width: 800px;
          margin: 0 auto;
          display: grid;
          grid-gap: 2rem;
          grid-template-columns: repeat(auto-fit, minmax(300px, 1fr));
      }

      .card {
          background-color: white;
          box-shadow: 2px 2px 12px 1px rgba(140, 140, 140, .5);
      }

      .card-title {
          font-size: 1.2rem;
          font-weight: bold;
          color: #034078
      }

      input[type=submit] {
          border: none;
          color: #FEFCFB;
          background-color: #034078;
          padding: 15px 15px;
          text-align: center;
          text-decoration: none;
          display: inline-block;
          font-size: 16px;
          width: 100px;
          margin-right: 10px;
          border-radius: 4px;
          transition-duration: 0.4s;
      }

      input[type=submit]:hover {
          background-color: #1282A2;
      }

      input[type=text],
      input[type=number],
      select {
          width: 50%;
          padding: 12px 20px;
          margin: 18px;
          display: inline-block;
          border: 1px solid #ccc;
          border-radius: 4px;
          box-sizing: border-box;
      }

      label {
          font-size: 1.2rem;
      }

      .value {
          font-size: 1.2rem;
          color: #1282A2;
      }

      .state {
          font-size: 1.2rem;
          color: #1282A2;
      }

      button {
          border: none;
          color: #FEFCFB;
          padding: 15px 32px;
          text-align: center;
          font-size: 16px;
          width: 100px;
          border-radius: 4px;
          transition-duration: 0.4s;
      }

      .button-on {
          background-color: #034078;
      }

      .button-on:hover {
          background-color: #1282A2;
      }

      .button-off {
          background-color: #858585;
      }

      .button-off:hover {
          background-color: #252524;
      }
    </style>
</head>

<body>
    <div class="topnav">
        <h1>COMMSenso</h1>
    </div>
    <div class="content">
        <div class="card-grid">
            <div class="card">
                <form action="/" method="POST">
                    <p>
                        <label for="ssid">SSID</label>
                        <input type="text" id="ssid" name="ssid"><br>
                        <label for="pass">Password</label>
                        <input type="text" id="pass" name="pass"><br>
                        <input type="submit" value="Submit">
                    </p>
                </form>
            </div>
        </div>
    </div>
</body>

</html>

)=====";


const char PAGE_CONTROL[] PROGMEM = R"=====(

<!DOCTYPE html>
<html>

<head>
    <title>COMMSENSO WEB SERVER</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link rel="icon" type="image/png" href="favicon.png">
    <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">


    <style>
      html {
          font-family: Arial, Helvetica, sans-serif;
          display: inline-block;
          text-align: center;
      }

      h1 {
          font-size: 1.8rem;
          color: white;
      }

      p {
          font-size: 1.4rem;
      }

      .topnav {
          overflow: hidden;
          background-color: #0A1128;
      }

      body {
          margin: 0;
      }

      .content {
          padding: 5%;
      }

      .card-grid {
          max-width: 800px;
          margin: 0 auto;
          display: grid;
          grid-gap: 2rem;
          grid-template-columns: repeat(auto-fit, minmax(300px, 1fr));
      }

      .card {
          background-color: white;
          box-shadow: 2px 2px 12px 1px rgba(140, 140, 140, .5);
      }

      .card-title {
          font-size: 1.2rem;
          font-weight: bold;
          color: #034078
      }

      input[type=submit] {
          border: none;
          color: #FEFCFB;
          background-color: #034078;
          padding: 15px 15px;
          text-align: center;
          text-decoration: none;
          display: inline-block;
          font-size: 16px;
          width: 100px;
          margin-right: 10px;
          border-radius: 4px;
          transition-duration: 0.4s;
      }

      input[type=submit]:hover {
          background-color: #1282A2;
      }

      input[type=text],
      input[type=number],
      select {
          width: 50%;
          padding: 12px 20px;
          margin: 18px;
          display: inline-block;
          border: 1px solid #ccc;
          border-radius: 4px;
          box-sizing: border-box;
      }

      label {
          font-size: 1.2rem;
      }

      .value {
          font-size: 1.2rem;
          color: #1282A2;
      }

      .state {
          font-size: 1.2rem;
          color: #1282A2;
      }

      button {
          border: none;
          color: #FEFCFB;
          padding: 15px 32px;
          text-align: center;
          font-size: 16px;
          width: 100px;
          border-radius: 4px;
          transition-duration: 0.4s;
      }

      .button-on {
          background-color: #034078;
      }

      .button-on:hover {
          background-color: #1282A2;
      }

      .button-off {
          background-color: #858585;
      }

      .button-off:hover {
          background-color: #252524;
      }
    </style>
</head>

<body>
    <div class="topnav">
        <h1>COMMSENSO</h1>
    </div>
    <div class="content">
        <div class="card-grid">
            <div class="card">
                <form action="/" method="get">
                    <p class="card-title"><i class="fas fa-lightbulb"></i>COMMSenso Sensores</p>
                    <input id="interval" name="interval" type="number" placeholder="Intervalo (milisegundos)">
                    <input id="container" name="container" type="text" placeholder="Container">
                    <p>
                        <a href="on"><button class="button-on" formaction="/on">ON</button></a>
                        <a href="off"><button class="button-off" formaction="/off">OFF</button></a>
                    </p>
                </form>
                <p class="state">State: %STATE%</p>
            </div>
        </div>
    </div>
</body>

)=====";


// Atualizar as variáveis do AsyncWebSite
String processor(const String& var) {
  if(var == "STATE") {
    if(Measuring) {
      MeasureState = "ON";
    }
    else {
      MeasureState = "OFF";
    }
    return MeasureState;
  }
  return String();
}


bool isConnected() {
    if (WiFi.status() == WL_CONNECTED) {
        return true;
    } 
    return false;
}


void startCredentialsServer() {
    // Definindo WiFi como ponto de acesso
    Serial.println("Setando ponto de acesso");  
    // Configuração do ponto de acesso
    WiFi.softAP("COMMSENSO", "COMMSENSO");

    IPAddress IP = WiFi.softAPIP();
    Serial.print("IP do ponto de acesso: ");
    Serial.println(IP); 

    //Página para colocar credenciais
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send_P(200, "text/html", PAGE_MAIN);
    });
    
    //Envio de dados
    server.on("/", HTTP_POST, [](AsyncWebServerRequest *request) {
      int params = request->params();
      for(int i=0;i<params;i++){
        AsyncWebParameter* p = request->getParam(i);
        if(p->isPost()){
          
          // HTTP POST ssid value
          if (p->name() == PARAM_INPUT_1) {
            ssid = p->value().c_str();
            Serial.print("SSID set to: ");
            Serial.println(ssid);
            
            // Write file to save value
            writeFile(ssidPath, ssid.c_str());
          }

          // HTTP POST pass value
          if (p->name() == PARAM_INPUT_2) {
            pass = p->value().c_str();
            Serial.print("Password set to: ");
            Serial.println(pass);

            // Write file to save value
            writeFile(passPath, pass.c_str());
          }
        }
      }

      request->send(200, "text/plain", "O ESP vai reiniciar e testar suas credenciais de conexão...");
      delay(3000);
      ESP.restart();


    });
    server.begin();
}

void startWebServer() {
    //Definindo Rotas
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
      if (request->hasParam(PARAM_INTERVAL)) {
        Serial.println(request->getParam(PARAM_INTERVAL)->value());
      }


      request->send_P(200, "text/html", PAGE_CONTROL, processor);
    });
    
    //Rota para iniciar a medição
    server.on("/on", HTTP_GET, [](AsyncWebServerRequest *request) {
      startMeasures();
      Measuring = true;
      
      if (request->hasParam(PARAM_INTERVAL)) {
        INTERVAL = std::stoi(request->getParam(PARAM_INTERVAL)->value().c_str());
      } 

      if (request->hasParam(PARAM_CONTAINER)) {
        CONTAINER = request->getParam(PARAM_CONTAINER)->value();
      }

      

      request->send_P(200, "text/html", PAGE_CONTROL, processor); 
    });

    //Rota para parar a medição
    server.on("/off", HTTP_GET, [](AsyncWebServerRequest *request) {
      stopMeasures();
      Measuring = false;
      request->send_P(200, "text/html", PAGE_CONTROL, processor);
    });


    server.begin();
}


bool initWiFi() {
  if(ssid==""){
    Serial.println("SSID indefinido");
    return false;
  }

  WiFi.begin(ssid.c_str(), pass.c_str());
  Serial.println("Conectando ao WiFi...");

  unsigned long currentMillis = millis();
  previousMillis = currentMillis;

  while(WiFi.status() != WL_CONNECTED) {
    currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      Serial.println("Falha na conexão");
      return false;
    }
  }

  Serial.println(WiFi.localIP());
  return true;
}


#endif

