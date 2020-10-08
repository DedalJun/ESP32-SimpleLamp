// Импортируем необходимые библиотеки:
#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"
#include "FastLED.h"

#define NUM_LEDS 5
#define PIN 27
int ledmode=1;
CRGB leds[NUM_LEDS];
byte counter = 0;

// Вставьте ниже SSID и пароль для своей WiFi-сети:
const char* ssid = "Masony";
const char* password = "22avatar8";

// Задаем GPIO-контакт, к которому подключен светодиод:
//const int ledPin = PIN;

// Создаем экземпляр класса «AsyncWebServer»
// под названием «server» и задаем ему номер порта «80»:
AsyncWebServer server(80);

String processor(const String& var) {
  // Создаем переменную для хранения состояния светодиода:
  String ledState = "228";


  return String();
}

void one_color_all(int cred, int cgrn, int cblu) {       //-SET ALL LEDS TO ONE COLOR
  for (int i = 0 ; i < NUM_LEDS; i++ ) {
    leds[i].setRGB( cred, cgrn, cblu);
  }
}

void kolhos(){
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CHSV(counter + i * 2, 255, 255);
  }
  counter++;
  //FastLED.show();
  //delay(5);
}

void random_burst() {                         //-m4-RANDOM INDEX/COLOR
  for(int i = 0; i < NUM_LEDS; i++){
    int color = random(0, 255);
    leds[i] = CHSV(color, 255, 255);
  }
}


void setup() {
  // Включаем последовательную коммуникацию (для отладки):
  Serial.begin(115200);
  pinMode(PIN, OUTPUT);

  // Инициализируем SPIFFS:
  if (!SPIFFS.begin(true)) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    //  "При монтировании SPIFFS произошла ошибка"
    return;
  }

  // Подключаемся к WiFi:
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
    //  "Подключаемся к WiFi..."
  }

  // Печатаем в мониторе порта локальный IP-адрес ESP32:
  Serial.println(WiFi.localIP());

  FastLED.addLeds<WS2812B, PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(50);

  // URL для корневой страницы веб-сервера:
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  // URL для файла «style.css»:
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/style.css", "text/css");
  });

  // URL для переключения GPIO-контакта на «HIGH»:
  server.on("/on", HTTP_GET, [](AsyncWebServerRequest * request) {
    ledmode=1;
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  // URL для переключения GPIO-контакта на «LOW»:
  server.on("/off", HTTP_GET, [](AsyncWebServerRequest * request) {
    ledmode=0;
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  server.on("/random", HTTP_GET, [](AsyncWebServerRequest * request) {
    ledmode=2;
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  // Запускаем сервер:
  server.begin();
}

void loop() {
  switch(ledmode){
    case 0: one_color_all(0,0,0); break;
    case 1: kolhos(); break;
    case 2: random_burst(); break;
  }
  FastLED.show();
  delay(5);
}
