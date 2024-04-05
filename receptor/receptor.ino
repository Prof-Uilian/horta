#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const uint64_t pipeIn = 0xE8E8F0F0E1LL;
RF24 radio(9, 10);

struct MyData {
  byte h;
  byte t;
};

MyData data;

void setup() {
  Serial.begin(9600);
  radio.begin();

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(F("Receiver"));
  display.display();
  delay(2000);

  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);
  radio.openReadingPipe(1, pipeIn);
  radio.startListening();
}

void recvData() {
  if (radio.available()) {
    radio.read(&data, sizeof(MyData));
  }
}

void loop() {
  recvData();

  Serial.print("Humidity: ");
  Serial.print(data.h);
  Serial.print("%, Temperature: ");
  Serial.print(data.t);
  Serial.println("C");

  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Umidade: ");
  display.print(data.h);
  display.println("%");

  display.setCursor(0, 16);
  display.print("Temperatura: ");
  display.print(data.t);
  display.println("C");

  display.display();

  delay(1000);
}
