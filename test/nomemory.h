#include <FS.h>          //this needs to be first, or it all crashes and burns...
#include <ESP8266WiFi.h> //https://github.com/esp8266/Arduino
// needed for library
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h> //https://github.com/tzapu/WiFiManager

#include <ArduinoJson.h> //https://github.com/bblanchon/ArduinoJson

#include <FastBot.h>
FastBot bot("160457790");

// define your default values here, if there are different values in config.json, they are overwritten.
char bot_token[50];
char chat_id[16];
// char blynk_token[34] = "YOUR_BLYNK_TOKEN";

// flag for saving data
bool shouldSaveConfig = false;

// callback notifying us of the need to save config
void saveConfigCallback()
{
    Serial.println("Should save config");
    shouldSaveConfig = true;
}

// обработчик сообщений
void newMsg(FB_msg &msg)
{
    Serial.println(msg.toString());

    // ответить
    bot.replyMessage("Hello!", msg.messageID, msg.chatID);
    // bot.sendMessage("start", msg.chatID);
}

void setup()
{
    // put your setup code here, to run once:
    Serial.begin(115200);
    Serial.println();

    // clean FS, for testing
    // SPIFFS.format();

    // read configuration from FS json
    Serial.println("mounting FS...");

    if (SPIFFS.begin())
    {
        Serial.println("mounted file system");
        if (SPIFFS.exists("/config.json"))
        {
            // file exists, reading and loading
            Serial.println("reading config file");
            File configFile = SPIFFS.open("/config.json", "r");
            if (configFile)
            {
                Serial.println("opened config file");
                size_t size = configFile.size();
                // Allocate a buffer to store contents of the file.
                std::unique_ptr<char[]> buf(new char[size]);

                configFile.readBytes(buf.get(), size);

#ifdef ARDUINOJSON_VERSION_MAJOR >= 6
                DynamicJsonDocument json(1024);
                auto deserializeError = deserializeJson(json, buf.get());
                serializeJson(json, Serial);
                if (!deserializeError)
                {
#else
                DynamicJsonBuffer jsonBuffer;
                JsonObject &json = jsonBuffer.parseObject(buf.get());
                json.printTo(Serial);
                if (json.success())
                {
#endif
                    Serial.println("\nparsed json");
                    strcpy(bot_token, json["bot_token"]);
                    strcpy(chat_id, json["chat_id"]);
                    // strcpy(blynk_token, json["blynk_token"]);
                }
                else
                {
                    Serial.println("failed to load json config");
                }
                configFile.close();
            }
        }
    }
    else
    {
        Serial.println("failed to mount FS");
    }
    // end read

    // The extra parameters to be configured (can be either global or just in the setup)
    // After connecting, parameter.getValue() will get you the configured value
    // id/name placeholder/prompt default length
    WiFiManagerParameter custom_bot_token("token", "telegram bot token", bot_token, 50);
    WiFiManagerParameter custom_chat_id("chatid", "telegram chat id", chat_id, 16);
    // WiFiManagerParameter custom_blynk_token("blynk", "blynk token", blynk_token, 32);

    // WiFiManager
    // Local intialization. Once its business is done, there is no need to keep it around
    WiFiManager wifiManager;

    // set config save notify callback
    wifiManager.setSaveConfigCallback(saveConfigCallback);

    // set static ip
    // wifiManager.setSTAStaticIPConfig(IPAddress(10, 0, 1, 99), IPAddress(10, 0, 1, 1), IPAddress(255, 255, 255, 0));

    // add all your parameters here
    wifiManager.addParameter(&custom_bot_token);
    wifiManager.addParameter(&custom_chat_id);
    // wifiManager.addParameter(&custom_blynk_token);

    // reset settings - for testing
    // wifiManager.resetSettings();

    // set minimu quality of signal so it ignores AP's under that quality
    // defaults to 8%
    // wifiManager.setMinimumSignalQuality();

    // sets timeout until configuration portal gets turned off
    // useful to make it all retry or go to sleep
    // in seconds
    // wifiManager.setTimeout(120);

    // fetches ssid and pass and tries to connect
    // if it does not connect it starts an access point with the specified name
    // here  "AutoConnectAP"
    // and goes into a blocking loop awaiting configuration
    if (!wifiManager.autoConnect("BulbLampAlarm", "password"))
    {
        Serial.println("failed to connect and hit timeout");
        delay(3000);
        // reset and try again, or maybe put it to deep sleep
        ESP.reset();
        delay(5000);
    }

    // if you get here you have connected to the WiFi
    Serial.println("connected...yeey :)");

    // read updated parameters
    strcpy(bot_token, custom_bot_token.getValue());
    strcpy(chat_id, custom_chat_id.getValue());
    // strcpy(blynk_token, custom_blynk_token.getValue());
    Serial.println("The values in the file are: ");
    Serial.println("\tbot_token : " + String(bot_token));
    Serial.println("\tchat_id : " + String(chat_id));
    // Serial.println("\tblynk_token : " + String(blynk_token));

    // save the custom parameters to FS
    if (shouldSaveConfig)
    {
        Serial.println("saving config");
#ifdef ARDUINOJSON_VERSION_MAJOR >= 6
        DynamicJsonDocument json(1024);
#else
        DynamicJsonBuffer jsonBuffer;
        JsonObject &json = jsonBuffer.createObject();
#endif
        json["bot_token"] = bot_token;
        json["chat_id"] = chat_id;
        // json["blynk_token"] = blynk_token;

        File configFile = SPIFFS.open("/config.json", "w");
        if (!configFile)
        {
            Serial.println("failed to open config file for writing");
        }

#ifdef ARDUINOJSON_VERSION_MAJOR >= 6
        serializeJson(json, Serial);
        serializeJson(json, configFile);
#else
        json.printTo(Serial);
        json.printTo(configFile);
#endif
        configFile.close();
        // end save
    }

    Serial.println("local ip");
    Serial.println(WiFi.localIP());

    // FastBot bot(bot_token);

    // bot.sendMessage("start", chat_id);
    // bot.attach(newMsg);
}

void loop()
{
    Serial.println(String(bot_token));
    Serial.println(String(chat_id));
    delay(3000);

    // bot.tick();
}