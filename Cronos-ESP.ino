#include <wiFi.h>
#include <WiFiClient.h>
#include <Arduino_JSON.h>

const char* ssid = "your-SSID"; // your network SSID (name)
const char* password = "your-PASSWORD"; // your network password

const char* server = "https://apex.oracle.com/pls/apex/projeto_7/api/"; // your server address

unsigned long lastTime = 0; // last time the server was contacted
unsigned long timerDelay = 5000; // delay between requests (5 seconds)

void setup() {
  serial.println("Iniciando o ESP32"); // initialize serial communication
  Serial.begin(115200); // initialize serial communication

}

void loop() {
  // put your main code here, to run repeatedly:

}


bool Connetion(){
  WiFi.begin(ssid, password); // connect to Wi-Fi
  Serial.println("Iniciando configurações de rede");

  while (WiFi.status() != WL_CONNECTED) { // wait for connection
    delay(500);
    Serial.println("Conectando a rede");
  }
  Serial.println("Conexão a rede realizada com sucesso!");

  Serial.println("");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
 
  Serial.println("As leituras são realizadas a cada" + timerDelay/1000 + "segundos");

  return 1; 
}