#ifndef MQTT_MANAGER_H
#define MQTT_MANAGER_H

#include <Arduino.h>
#include <PubSubClient.h>
#include <AsyncTCP.h>


const char* mqtt_server = "192.168.1.104";
WiFiClient espClient;
PubSubClient client(espClient);

#define MSG_BUFFER_SIZE 50
char msg[MSG_BUFFER_SIZE];

char* topic = "/commsenso/send-measure";




//MQTT FUNCTIONS
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message Arrived [");
  Serial.print(topic);
  Serial.print("]: ");
  for (int i = 0; i < length;i++) {
    Serial.print((char) payload[i]);
  }

  Serial.println();
}

void reconnect() {

  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP32";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");  
      delay(5000);

    }
  }
}

void publish(char *message) {
    client.publish(topic, message);
}


bool mqttConnected() {
    if (client.connected()) {
        return true;
    }
    return false;
}

void configureMqttServer() {

    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);
    client.setKeepAlive(60);
}


#endif
