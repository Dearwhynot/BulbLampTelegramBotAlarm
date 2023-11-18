#include <Arduino.h>

#include <ESP8266WiFi.h>

const char* ssid = "5030378024";     // Имя вашей Wi-Fi сети
const char* password = "00073693"; // Пароль Wi-Fi сети

void setup() {
  Serial.begin(115200);
  Serial.println("Start Serial");

  // Подключение к Wi-Fi
  WiFi.begin(ssid, password);

  // Ожидание подключения к Wi-Fi
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");
}

void loop() {
  // Ваш код здесь
}
