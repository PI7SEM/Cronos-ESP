#include <WiFi.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <Adafruit_NeoPixel.h>

const char *ssid = "CACHORRO";       // your network SSID (name)
const char *password = "hi12345678"; // your network password

const char *server = "URL API HTTP"; // your server address

unsigned long lastTime = 0;      // last time the server was contacted
unsigned long timerDelay = 5000; // delay between requests (5 seconds)

#define PIN_L1 5          // Pino conectado ao NeoPixel (ajuste conforme necessário)
#define NUM_LEDS_L1 16    // Número de LEDs no anel

#define PIN_L2 18          // Pino conectado ao NeoPixel (ajuste conforme necessário)
#define NUM_LEDS_L2 16    // Número de LEDs no anel

#define PIN_L3 19          // Pino conectado ao NeoPixel (ajuste conforme necessário)
#define NUM_LEDS_L3 16    // Número de LEDs no anel

Adafruit_NeoPixel strip1 = Adafruit_NeoPixel(NUM_LEDS_L1, PIN_L1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(NUM_LEDS_L2, PIN_L2, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip3 = Adafruit_NeoPixel(NUM_LEDS_L3, PIN_L3, NEO_GRB + NEO_KHZ800);

void setup()
{
    Serial.println("Iniciando o ESP32"); // initialize serial communication
    Serial.begin(115200);                // initialize serial communication

    setupNeoPixel(strip1);
    setupNeoPixel(strip2);
    setupNeoPixel(strip3);

    bool connected = connection(); // check if connected to Wi-Fi'

    if (connected)
    {
        delay(1000);                                                // wait for 1 second
        Serial.println("Dispositivo pronto para iniciar operação"); // connected to Wi-Fi
    }
    else
    {
        Serial.println("Falha na conexão com a rede Wi-Fi!"); // failed to connect ''to Wi-Fi
    }
}

void setupNeoPixel(Adafruit_NeoPixel lum) {
    lum.begin();       // Inicializa o NeoPixel
    lum.show();        // Garante que todos os LEDs estejam apagados inicialmente
  }

void loop(){

    String data = getRequest(); // get request from server

    if (data == "000") {
        controlNeoPixel(strip1, false);
        controlNeoPixel(strip2, false);
        controlNeoPixel(strip3, false);
    } else if (data == "001") {
        controlNeoPixel(strip1, false);
        controlNeoPixel(strip2, false);
        controlNeoPixel(strip3, true);
    } else if (data == "010") {
        controlNeoPixel(strip1, false);
        controlNeoPixel(strip2, true);
        controlNeoPixel(strip3, false);
    } else if (data == "011") {
        controlNeoPixel(strip1, false);
        controlNeoPixel(strip2, true);
        controlNeoPixel(strip3, true);
    } else if (data == "100") {
        controlNeoPixel(strip1, true);
        controlNeoPixel(strip2, false);
        controlNeoPixel(strip3, false);
    } else if (data == "101") {
        controlNeoPixel(strip1, true);
        controlNeoPixel(strip2, false);
        controlNeoPixel(strip3, true);
    } else if (data == "110") {
        controlNeoPixel(strip1, true);
        controlNeoPixel(strip2, true);
        controlNeoPixel(strip3, false);
    } else if (data == "111") {
        controlNeoPixel(strip1, true);
        controlNeoPixel(strip2, true);
        controlNeoPixel(strip3, true);
    } else {
        controlNeoPixel(strip1, false);
        controlNeoPixel(strip2, false);
        controlNeoPixel(strip3, false);
    }

}

    bool connection(){
    WiFi.begin(ssid, password); // connect to Wi-Fi
    Serial.println("Iniciando configurações de rede");

    while (WiFi.status() != WL_CONNECTED)
    { // wait for connection
        delay(500);
        Serial.println("Conectando a rede");
    }
    Serial.println("Conexão a rede realizada com sucesso!");

    Serial.println("");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());

    Serial.println("As leituras são realizadas a cada" + String(timerDelay / 1000) + "segundos");

    return WiFi.status() == WL_CONNECTED;
}

String getRequest(){
    if ((millis() - lastTime) > timerDelay)
    {
        lastTime = millis(); // Atualiza o tempo da última requisição

        HTTPClient http;    // Cria um objeto HTTPClient
        http.begin(server); // Define o endpoint do servidor

        int httpResponseCode = http.GET(); // Faz a requisição GET

        if (httpResponseCode > 0)
        {                                      // Verifica se a requisição foi bem-sucedida
            String payload = http.getString(); // Obtém a resposta como string
            Serial.println("Resposta do servidor: " + payload);
            http.end();     // Finaliza a conexão HTTP
            return payload; // Retorna a resposta do servidor
        }
        else
        {
            Serial.print("Erro na requisição HTTP: ");
            Serial.println(httpResponseCode);
        }

        http.end(); // Finaliza a conexão HTTP
    }

    return "000"; // Retorna uma string vazia caso o timer não tenha expirado ou ocorra erro
}

void controlNeoPixel(Adafruit_NeoPixel lum, bool state) {
    int pixels = 16;
    if (state) {
      // Liga todos os LEDs com uma cor (exemplo: vermelho)
      for (int i = 0; i < pixels; i++) {
        lum.setPixelColor(i, lum.Color(255, 0, 0)); // Define a cor (R, G, B)
      }
    } else {
      // Desliga todos os LEDs
      for (int i = 0; i < pixels; i++) {
        lum.setPixelColor(i, lum.Color(0, 0, 0)); // Define a cor como apagado
      }
    }
    lum.show(); // Atualiza os LEDs para refletir as mudanças
  }
