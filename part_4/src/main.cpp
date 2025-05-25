#include <Adafruit_NeoPixel.h>
#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
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

QueueHandle_t my_queue;

void input_accepter(void *param) {
  while (1) {
    if (Serial.available() > 0) {
      String recieved = Serial.readString();
      char *ptr_to_string = (char *)pvPortMalloc(recieved.length() + 1);
      if (ptr_to_string == NULL) {
        Serial.println("!!! Allocaiton Failed !!!");
      }

      strcpy(ptr_to_string, recieved.c_str());

      xQueueSend(my_queue, &ptr_to_string, 0);
      vTaskDelay(pdTICKS_TO_MS(10));
      ptr_to_string = NULL;
    }
  }
}

void input_printer(void *param) {
  while (1) {

    char *received_pointer;

    if (xQueueReceive(my_queue, &received_pointer, portMAX_DELAY) == pdTRUE) {
      Serial.print("Must be the word: ");

      Serial.println(received_pointer);

      vPortFree(received_pointer);
      received_pointer = NULL;
    }
  }
}

void setup() {

  Serial.begin(9600);
  pixels.begin();
  pixels.setPixelColor(0, pixels.Color(255, 0, 0));
  pixels.show();

  my_queue = xQueueCreate(1, sizeof(char *));

  xTaskCreatePinnedToCore(input_accepter, "blinker", 4096, NULL, 2, NULL,
                          app_cpu);
  xTaskCreatePinnedToCore(input_printer, "blinker", 4096, NULL, 1, NULL,
                          app_cpu);
}

void loop() {}
