#include <ESP8266WiFi.h>

// датчик движения
// "HC-SR505" 
// umnyjdomik.ru
int led = 2 ;// назначение пина для светодиода
int pirPin = 0; // назначение пина для мини ИК датчика
int value ;// переменная для хранения положения датчика
void setup ()
{
  Serial.begin(115200);
  pinMode (led, OUTPUT) ;// пин светодиода работает как выход
  pinMode (pirPin, INPUT) ; // пин датчика работает как вход
}
void loop ()
{
  value = digitalRead (pirPin) ;// чтение значения с датчика
  if (value == HIGH) // когда с ИК сенсора появляется высокий уровень, светодиод загорается
  {
    digitalWrite (led, LOW);
    Serial.println("movement");
  }
  else
  {
    digitalWrite (led, HIGH);
    Serial.println("no movement");
  }
  delay(1000);
}