#include <Arduino.h>

// бот отвечает на сообщения из любого чата
#define WIFI_SSID "5030378024"
#define WIFI_PASS "00073693"
#define BOT_TOKEN "468671560:AAFm-DVnQp0ky81rXY79pOYoewX2Od9x68k"
#define CHAT_ID "160457790"

#include <FastBot.h>
FastBot bot(BOT_TOKEN);

#include <EEPROM.h>

// датчик движения
// "HC-SR505"
// umnyjdomik.ru
int led = 2;    // назначение пина для светодиода
int pirPin = 0; // назначение пина для мини ИК датчика
int value;      // переменная для хранения положения датчика

unsigned long previousMillis = 0; // Сохраняет время последнего вывода сообщения
const long interval = 10000;      // Интервал в миллисекундах (10 секунд)

void connectWiFi()
{
    delay(2000);
    Serial.begin(115200);
    Serial.println();

    WiFi.begin(WIFI_SSID, WIFI_PASS);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
        if (millis() > 15000)
            ESP.restart();
    }
    Serial.println("Connected");
}

void saveAlarmStatus(bool status)
{
    EEPROM.begin(1); // Инициализация EEPROM с размером 512 байт
    EEPROM.put(0, status);
    EEPROM.commit(); // Сохранение изменений
    EEPROM.end();    // Завершение работы с EEPROM
}

bool alarmed()
{
    bool valueRead;
    EEPROM.begin(1);
    EEPROM.get(0, valueRead);
    EEPROM.end();
    return valueRead;
}

// обработчик сообщений
void newMsg(FB_msg &msg)
{
    Serial.println(msg.toString());

    if (msg.text == "/stop")
    {
        bot.sendMessage("Alarm Bot Stoped", CHAT_ID);
        saveAlarmStatus(false);
    }
    else if (msg.text == "/start")
    {
        bot.sendMessage("Alarm Bot Started", CHAT_ID);
        saveAlarmStatus(true);
    }

    // ответить
    // bot.replyMessage("Hello!", msg.messageID, msg.chatID);
    // bot.sendMessage("start", msg.chatID);
}

void setup()
{
    connectWiFi();
    bot.sendMessage("start", CHAT_ID);
    bot.attach(newMsg);

    pinMode(led, OUTPUT);   // пин светодиода работает как выход
    pinMode(pirPin, INPUT); // пин датчика работает как вход
}

void loop()
{
    bot.tick();

    value = digitalRead(pirPin); // чтение значения с датчика
    if (value == HIGH)           // когда с ИК сенсора появляется высокий уровень, светодиод загорается
    {
        digitalWrite(led, LOW);
        Serial.println("movement");
        if (alarmed())
        {
            unsigned long currentMillis = millis();
            // Проверяем прошедшее время с момента последнего вывода сообщения
            if (currentMillis - previousMillis >= interval)
            {
                // Сохраняем текущее время для следующего цикла
                previousMillis = currentMillis;
                // Выводим ваше сообщение
                Serial.println("Sending bot msg");
                bot.sendMessage("Alarm movement!", CHAT_ID);
            }
            Serial.println("Alarmed");
        }
        else
        {
            Serial.println("Not alarmed");
        }
    }
    else
    {
        digitalWrite(led, HIGH);
        Serial.println("no movement");
    }
    delay(1000);
}