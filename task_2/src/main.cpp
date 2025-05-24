#include <Adafruit_NeoPixel.h>
#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <stdlib.h>

#define NUM_PIXELS 1
#define DATA_PIN 38

Adafruit_NeoPixel pixels(NUM_PIXELS, DATA_PIN, NEO_GRB + NEO_KHZ800);

#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

int blink_interval = 1000;

void blinker_task(void *param) {
  while (1) {
    pixels.setPixelColor(0, pixels.Color(0, 128, 128));
    pixels.show();
    Serial.println("on");
    vTaskDelay(pdMS_TO_TICKS(blink_interval));

    pixels.setPixelColor(0, pixels.Color(0, 0, 0));
    pixels.show();
    Serial.println("off");
    vTaskDelay(pdMS_TO_TICKS(blink_interval));
  }
}

void speed_input_handler(void *params) {
  while (1) {

    if (Serial.available() > 0) {
      int input_string = atoi(Serial.readString().c_str());
      blink_interval = input_string;
    }
  }
}

void setup() {

  pixels.begin();
  pixels.setPixelColor(0, pixels.Color(255, 0, 0));
  pixels.show();

  Serial.begin(9600);

  xTaskCreatePinnedToCore(blinker_task, "blinker", 4096, NULL, 2, NULL,
                          app_cpu);

  xTaskCreatePinnedToCore(speed_input_handler, "speed_controller", 4096, NULL,
                          1, NULL, app_cpu);
}

void loop() {}