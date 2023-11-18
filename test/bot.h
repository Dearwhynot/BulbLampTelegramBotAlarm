#include <Arduino.h>

// бот отвечает на сообщения из любого чата
#define WIFI_SSID "5030378024"
#define WIFI_PASS "00073693"
#define BOT_TOKEN "468671560:AAFm-DVnQp0ky81rXY79pOYoewX2Od9x68k"
#define CHAT_ID "160457790"

#include <FastBot.h>
FastBot bot(BOT_TOKEN);

void connectWiFi() {
  delay(2000);
  Serial.begin(115200);
  Serial.println();

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (millis() > 15000) ESP.restart();
  }
  Serial.println("Connected");
}

// обработчик сообщений
void newMsg(FB_msg& msg) {
  Serial.println(msg.toString());

  // ответить
  bot.replyMessage("Hello!", msg.messageID, msg.chatID);  
  // bot.sendMessage("start", msg.chatID);
}

void setup() {
  connectWiFi();
  bot.sendMessage("start", CHAT_ID);
  bot.attach(newMsg);
}

void loop() {
  bot.tick();
}