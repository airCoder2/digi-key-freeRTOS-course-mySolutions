#include <Adafruit_NeoPixel.h>
#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include <freertos/task.h>
#include <stdint.h>
#include <stdlib.h>

#define NUM_PIXELS 1
#define DATA_PIN 38

Adafruit_NeoPixel pixels(NUM_PIXELS, DATA_PIN, NEO_GRB + NEO_KHZ800);


/**
 * FreeRTOS Mutex Challenge
 *
 * Pass a parameter to a task using a mutex.
 *
 * Date: January 20, 2021
 * Author: Shawn Hymel
 * License: 0BSD
 */

// You'll likely need this on vanilla FreeRTOS
// #include <semphr.h>

// Use only core 1 for demo purposes
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

// Pins (change this if your Arduino board does not have LED_BUILTIN defined)
static const int led_pin = LED_BUILTIN;

SemaphoreHandle_t my_mutex;

//*****************************************************************************
// Tasks

// Blink LED based on rate passed by parameter
void blinkLED(void *parameters)
{
  // Copy the parameter into a local variable

  int num = *(int *) parameters;

  // Print the parameter
  Serial.print("Received: ");
  Serial.println(num);

  // Configure the LED pin
  pinMode(led_pin, OUTPUT);

  // Blink forever and ever
  while (1)
  {
    pixels.setPixelColor(0, pixels.Color(0, 128, 128));
    pixels.show();
    vTaskDelay(pdMS_TO_TICKS(num));

    pixels.setPixelColor(0, pixels.Color(0, 0, 0));
    pixels.show();
    vTaskDelay(pdMS_TO_TICKS(num));

  }
}

//*****************************************************************************
// Main (runs as its own task with priority 1 on core 1)

void setup()
{
  pixels.begin();
  pixels.setPixelColor(0, pixels.Color(255, 0, 0));
  pixels.show();

  my_mutex = xSemaphoreCreateMutex();

  if (xSemaphoreTake(my_mutex, 0)){
    Serial.println("mutex taken");
  }

  // Configure Serial
  Serial.begin(9600);

  // Wait a moment to start (so we don't miss Serial output)

  vTaskDelay(8000 / portTICK_PERIOD_MS);
  Serial.println();
  Serial.println("---FreeRTOS Mutex Challenge---");
  Serial.println("Enter a number for delay (milliseconds)");

  // Wait for input from Serial
  while (Serial.available() <= 0)
    ;

    long int delay_arg;
    delay_arg = Serial.parseInt();
    Serial.print("Sending: ");
    Serial.println(delay_arg);

  // Start task 1
  xTaskCreatePinnedToCore(blinkLED, "Blink LED", 4096, (void *) &delay_arg, 1, NULL, app_cpu);

  // Show that we accomplished our task of passing the stack-based argument
  Serial.println("Done!");

  xSemaphoreTake(my_mutex, portMAX_DELAY);

  }

void loop()
{

  // Do nothing but allow yielding to lower-priority tasks
  vTaskDelay(1000 / portTICK_PERIOD_MS);
}