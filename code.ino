/*
#AUTHOR Дмитрий Витальевич Мельник
#MAIL i.x.c.o.n@yandex.ru
#LICENSE Использование  только  для не коммерческих проектов, без упоминания автора. При использовании автор не несет ни какой отвественности, и не дает ни какой гарантии работы. Не ведет поддержку или исправление ошибок. 
*/
/*
Суть  проекта: контроль температуры с помощью  датчика
Вывод значения  на  дисплей.
Если температуры превышает  лимит то подача звукового сигнала
*/
#include <OneWire.h>
#include <DallasTemperature.h>

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // Ширина OLED-дисплея, в пикселях
#define SCREEN_HEIGHT 64 // Высота OLED-дисплея, в пикселях
// Декларация для дисплея SSD1306, подключенного к I2C (контакты SDA, SCL)
#define OLED_RESET -1 // Сброс pin-кода # (или -1, если используется общий pin-код сброса Arduino)

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define PINTONE A1 //Зуммер

//DS18B20 от  -55 до +125
#define ONE_WIRE_BUS 2
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

#define LIMITTEMP 110.00

uint32_t timer = 0; //Таймер для псевдо много поточности
uint32_t timer1 = 0; //Таймер для псевдо много поточности
uint32_t timer2 = 0; //Таймер для псевдо много поточности

float temp = 0;

void setup() {
  Serial.begin(115200);
  Serial.println("Start OK");
  sensors.begin();
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
  }
  display.clearDisplay(); // Чистим буфер
  display.setCursor(12, 28);
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.println("START OK!");
  display.display();
  delay(1000);
  
}

void loop() {
  
  if(millis() - timer >= 3000){//START 1
    timer = millis();
    //Serial.println("Timer OK");
    sensors.requestTemperatures();
    temp = sensors.getTempCByIndex(0);
    Serial.println(temp);
    //Serial.println(sensors.getTempCByIndex(0));

  }//END 1

  if(millis() - timer1 >= 1000){//START 2
    timer1 = millis();
    //Serial.println("Timer1 OK");
    if(LIMITTEMP < temp){//Тут мы проверяем есть  ли у нас привышение лимита
      tone(PINTONE, 1000, 500); //Если есть проигрываем звук
    }    
  }//END 2

  if(millis() - timer2 >= 4000){//START 4
    timer2 = millis();
    //Serial.println("Timer3 OK");
    display.clearDisplay(); // Чистим буфер
    display.setCursor(12, 28);
    display.setTextColor(WHITE);
    display.setTextSize(2);
    char tempstr[16];
    dtostrf(temp, 4, 2, tempstr);
    strcat(tempstr, " C\n");
    display.println(tempstr);
    display.display();
  }//END 4  

}
