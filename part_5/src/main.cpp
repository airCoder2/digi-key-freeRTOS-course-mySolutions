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

#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

struct blinky_updated
{
  char message[32] = "100 more times!";
  int times_blinked;
};

typedef struct blinky_updated blinky_updated;

QueueHandle_t queue_1;
QueueHandle_t queue_2;

// should be able to read the data from serial monitor.
// if delay xxx
void input_accepter_interval_parser_and_queue1_updated(void *param)
{
	blinky_updated received_struct;
  while (1)
  {
    if (xQueueReceive(queue_2, &received_struct, 0) == pdTRUE)
    {
			Serial.println(received_struct.message);
			Serial.print("Total blinks: ");
			Serial.println(received_struct.times_blinked);
    }

    if (Serial.available() > 0)
    {
      String recieved = Serial.readString();
      recieved.trim();
      char *c_recieved = (char *) malloc(strlen(recieved.c_str()) + 1);
      strcpy(c_recieved, recieved.c_str()); // from now on, I only work with c_recieved

      char key_word[] = "delay";
      char *key_word_position =
          strstr(c_recieved, key_word); // search for that word inside the user input

      if (key_word_position != NULL)
      { // if the keyword 'delay' exists
        int position =
            (int) (key_word_position - c_recieved); // get the index of the keyword 'delay'
        if (position == 0 && *(c_recieved + strlen(key_word)) == ' ')
        {

          // we are seeing if the keyword appears at the beginning and if there
          // is a space after the keyword itself
          int idx = 0;
          char interval[8]; // array to store the word after space + keyword
          memset(interval, 0, sizeof(interval));
          // after the space, dump the next substring until the next space
          // inside the interval array
          while (*(c_recieved + strlen(key_word) + idx + 1) != '\0' &&
                 *(c_recieved + strlen(key_word) + idx + 1) != ' ')
          {
            interval[idx] = *(c_recieved + strlen(key_word) + idx + 1);
            // Serial.println(idx);
            idx++;
          }
          if (idx != 0)
          { // means if while loop has been used

            char *endptr = NULL; // I can check later if the conversion was
                                 // sucessfull all the way
            int interval_d = (int) strtol(interval,
                                          &endptr,
                                          10); // converting the parsed interval into a number
            // Serial.print("unfiltered parse: ");
            // Serial.println(interval_d);

            if (*endptr == '\0')
            { // if the conversion was successful (full conversion)

              xQueueSend(queue_1, &interval_d,
                         0); // put the item inside the queue
							free(c_recieved);
              Serial.println("Alhamdulillah!");
            }
            else
            {
              Serial.println("plase enter a valid interval rate. Ex: delay 100");
            }
          }
        }
        else
        {
          Serial.println("keyword delay does not occur at the beginning of the "
                         "word, or does not have space after it");
        }
      }
      else
      {
        Serial.println(c_recieved);
				free(c_recieved);
      }
    }
  }
}

void interval_setter_queue2_updater(void *param)
{
  int received_interval = 1000; // initially
	int blinked_times = 0;
	blinky_updated send_struct;
  while (1)
  {
    if (xQueueReceive(queue_1, &received_interval, 0) == pdTRUE)
    {
      Serial.print("Updated interval rate: ");
      Serial.println(received_interval);
    } 

		pixels.setPixelColor(0, pixels.Color(0, 128, 128));
    pixels.show();
    vTaskDelay(pdMS_TO_TICKS(received_interval));

    pixels.setPixelColor(0, pixels.Color(0, 0, 0));
    pixels.show();
    vTaskDelay(pdMS_TO_TICKS(received_interval));

		blinked_times = blinked_times + 1;
		
		if ((blinked_times % 100) == 0){
			// Serial.println("sent the struct");
			send_struct.times_blinked = blinked_times;
			xQueueSend(queue_2, &send_struct, portMAX_DELAY);
		}

  }
}

void setup()
{
  Serial.begin(9600);
  pixels.begin();
  pixels.setPixelColor(0, pixels.Color(255, 0, 0));
  pixels.show();

  queue_1 = xQueueCreate(5, sizeof(int));
  queue_2 = xQueueCreate(5, sizeof(blinky_updated));

  xTaskCreatePinnedToCore(
      input_accepter_interval_parser_and_queue1_updated, "blinker", 4096, NULL, 1, NULL, app_cpu);
  xTaskCreatePinnedToCore(interval_setter_queue2_updater, "blinker", 4096, NULL, 2, NULL, app_cpu);
}

void loop() {}
