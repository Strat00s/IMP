#include <Arduino.h>
#include <FreeRTOS.h>
#include <freertos/queue.h>
#include <freertos/task.h>

QueueHandle_t queue;

void ledTask(void *params) {
    int button = 0;
    while(true) {
        if (uxQueueMessagesWaiting(queue)) {
            xQueueReceive(queue, &button, 0);
        }
        for (int i = 0; i < button*2; i ++) {
            digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
            printf("%d %d\n", i, digitalRead(LED_BUILTIN));
            vTaskDelay(100);
        }
        vTaskDelay(1000);
    }
}

void addToQueue(int data) {
    xQueueSend(queue, &data, portMAX_DELAY);
}

void buttonTask(void *params) {
    while(true) {
        if (!digitalRead(16)) addToQueue(1);
        if (!digitalRead(17)) addToQueue(5);
        vTaskDelay(100);
    }
}

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(16, INPUT_PULLUP);
    pinMode(17, INPUT_PULLUP);
    queue = xQueueCreate(1, sizeof(int));
    xTaskCreate(ledTask, "led_task", 10000, NULL, 1, NULL);
    xTaskCreate(buttonTask, "button_task", 10000, NULL, 1, NULL);
}

void loop() {
}