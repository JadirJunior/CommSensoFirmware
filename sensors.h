#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>
#include <SoftwareSerial.h>


#define RE 19
#define DE 18
#define PIN_RX 16
#define PIN_TX 17

uint8_t temp[] = {0x01, 0x03, 0x02, 0x00, 0x00, 0x01, 0x00, 0x00};
uint8_t umid[] = {0x01, 0x03, 0x02, 0x01, 0x00, 0x01, 0x00, 0x00};
uint8_t nitro[] = {0x01, 0x03, 0x02, 0x04, 0x00, 0x01, 0x00, 0x00};
uint8_t phos[] = {0x01, 0x03, 0x02, 0x05, 0x00, 0x01, 0x00, 0x00};
uint8_t Ph[] = {0x01, 0x03, 0x02, 0x03, 0x00, 0x01, 0x00, 0x00};
uint8_t cond[] = {0x01, 0x03, 0x02, 0x02, 0x00, 0x01, 0x00, 0x00};
uint8_t phota[] = {0x01, 0x03, 0x02, 0x06, 0x00, 0x01, 0x00, 0x00};

// Configuração da serial usando SoftwareSerial
SoftwareSerial mod(PIN_RX, PIN_TX); // RX, TX


enum sensorsIds {
  TEMPERATURE = 1,
  UMIDITY = 2,
  CONDUCTIVITY = 3,
  PH = 4,
  NITROGEN = 5,
  PHOSPHORUS = 6,
  POTASSIUM = 7
};

struct ResponseMeasure {
  int idSensor;
  float value;
};

uint16_t crc16(const uint8_t *data, uint16_t length) {
  uint16_t crc = 0xFFFF;

  for (uint16_t pos = 0; pos < length; pos++) {
    crc ^= (uint16_t)data[pos];    // XOR byte into least sig. byte of crc

    for (uint8_t i = 8; i != 0; i--) {    // Loop over each bit
      if ((crc & 0x0001) != 0) {    // If the LSB is set
        crc >>= 1;    // Shift right and XOR 0xA001
        crc ^= 0xA001;
      } else {    // Else LSB is not set
        crc >>= 1;    // Just shift right
      }
    }
  }

  return crc;
}

void calculateCRC(uint8_t *command, size_t length) {
  uint16_t crc = crc16(command, length - 2);
  command[length - 2] = crc & 0xFF;
  command[length - 1] = (crc >> 8) & 0xFF;
}

void byteToHex(uint8_t byte, char* hexBuffer) {
  const char hexDigits[] = "0123456789ABCDEF";
  hexBuffer[0] = hexDigits[(byte >> 4) & 0x0F];
  hexBuffer[1] = hexDigits[byte & 0x0F];
  hexBuffer[2] = '\0';
}

ResponseMeasure getMeasure(uint8_t query[], float entangle, int sensorId) {
  
  char values[15]; // Array para armazenar os valores hexadecimais
  int cont=0;
  digitalWrite(DE,HIGH);
  digitalWrite(RE,HIGH);
  delay(10);

  mod.flush();
  while (mod.available()) {
    mod.read();
  }


  if(mod.write(query,8)==8){
    delay(50);
    digitalWrite(DE,LOW);
    digitalWrite(RE,LOW);
    for(byte i=0;i<7;i++){

      unsigned long startTime = millis();

      while (!mod.available() && millis() - startTime < 1000UL) {
        // Aguarda até que os dados estejam disponíveis ou o timeout ocorra
      }

      uint8_t byteRead = mod.read();
      
      char hexValue[3]; // Buffer para armazenar valor hexadecimal
      byteToHex(byteRead, hexValue);

      values[cont] = hexValue[0];
      values[cont + 1] = hexValue[1];

      // Imprimir o byte em formato hexadecimal
      // Serial.print(hexValue);
      // Serial.print(' ');

      cont += 2;
    }
  }

  mod.flush();

  values[cont] = '\0';

  ResponseMeasure res;

  res.idSensor = sensorId;

  if (cont == 14) {
    char responseHex[5] = {values[6], values[7], values[8], values[9], '\0'};
    uint16_t value = strtol(responseHex, NULL, 16);

    float valueFloat = static_cast<float>(value)/entangle;

    res.value = valueFloat;
  } else {
    res.value = -1;
  }

  return res;

}


void setupSensors() {
  mod.begin(9600);

  pinMode(RE, OUTPUT);
  pinMode(DE, OUTPUT);
  digitalWrite(DE, LOW);
  digitalWrite(RE, LOW);

  calculateCRC(temp, sizeof(temp));
  calculateCRC(umid, sizeof(umid));
  calculateCRC(nitro, sizeof(nitro));
  calculateCRC(phos, sizeof(phos));
  calculateCRC(Ph, sizeof(Ph));
  calculateCRC(cond, sizeof(cond));
  calculateCRC(phota, sizeof(phota));
  delay(500);
}

#endif
