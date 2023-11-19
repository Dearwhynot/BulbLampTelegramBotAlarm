#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>
#include <DNSServer.h>
#include <WiFiManager.h>

const int ledPin = 2; // GPIO2 на ESP-01, к которому подключен светодиод

ESP8266WebServer httpServer(80);
ESP8266HTTPUpdateServer httpUpdater;

void setup()
{
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, HIGH); // Выключаем светодиод

    // Настройка Wi-Fi с помощью WiFiManager
    WiFiManager wifiManager;
    wifiManager.autoConnect("ESP-01-OTA-Setup"); // Если не удалось подключиться к Wi-Fi, создаем точку доступа

    // Выводим IP-адрес в монитор последовательного порта
    Serial.begin(115200);
    Serial.println("Connected to Wi-Fi");
    Serial.print("IP Address: http://");
    Serial.print(WiFi.localIP());
    Serial.println("/update");

    // Настройка OTA
    MDNS.begin("esp-01");
    httpUpdater.setup(&httpServer);
    httpServer.begin();
    MDNS.addService("http", "tcp", 80);

    Serial.println("OTA Update Server Started");
}

void loop()
{
    httpServer.handleClient();

    // Ваш код
    digitalWrite(ledPin, HIGH); // Включаем светодиод
    delay(1000);
    digitalWrite(ledPin, LOW); // Выключаем светодиод
    delay(1000);
}
