
#ifndef CONFIGURE_SPIFFS_H
#define CONFIGURE_SPIFFS_H
#include <Arduino.h>


#include "SPIFFS.h"


// SPIFFS FUNCTIONS
void initSPIFFS() {
  if (!SPIFFS.begin(true)) {
    Serial.println("An error has occurred while mounting SPIFFS");
  }
  Serial.println("SPIFFS mounted successfully");
}

// Ler arquivo do SPIFFS
String readFileSpiffs(fs::FS &fs, const char * path){
  Serial.printf("Reading file: %s\r\n", path);

  File file = fs.open(path);

  // if (file) {
  //   SPIFFS.remove(path);
  // }


  if(!file || file.isDirectory()){
    Serial.println("- failed to open file for reading");
    return String();
  }

  String fileContent;
  while(file.available()){
    fileContent = file.readStringUntil('\n');
    break;
  }

  return fileContent;
}

void writeFileSpiffs(fs::FS &fs, const char * path, const char * message){
  Serial.printf("Writing file: %s\r\n", path);

  File file = fs.open(path, FILE_WRITE);
  if(!file){
    Serial.println("- failed to open file for writing");
    return;
  }
  if(file.print(message)){
    Serial.println("- file written");
  } else {
    Serial.println("- write failed");
  }
}

String readFile(const char * path) {
  return readFileSpiffs(SPIFFS, path);
}

void writeFile(const char * path, const char * message) {
  writeFileSpiffs(SPIFFS, path, message);
}


// Escrever arquivo no SPIFFS



#endif

