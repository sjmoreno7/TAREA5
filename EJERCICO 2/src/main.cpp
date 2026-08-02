#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define LED_PIN GPIO_NUM_21

// Variables globales para compartir estado entre tareas
int sensor_value = 0;
bool led_state = false;

// Tarea 1: Lectura de sensor virtual (Frecuencia: 100ms)
void vTaskSensor(void *pvParameters) {
    while(1) {
        sensor_value = (sensor_value + 1) % 100; // Simula la variación de un sensor
        vTaskDelay(pdMS_TO_TICKS(100)); 
    }
}

// Tarea 2: Control de salida digital / LED (Frecuencia: 500ms)
void vTaskLED(void *pvParameters) {
    gpio_reset_pin(LED_PIN);
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);
    
    while(1) {
        led_state = !led_state;
        gpio_set_level(LED_PIN, led_state);
        vTaskDelay(pdMS_TO_TICKS(500)); 
    }
}

// Tarea 3: Envío periódico al monitor serial (Frecuencia: 2000ms)
void vTaskSerial(void *pvParameters) {
    while(1) {
        printf("Reporte FreeRTOS -> Sensor: %d | Estado LED: %s\n", sensor_value, led_state ? "ENCENDIDO" : "APAGADO");
        vTaskDelay(pdMS_TO_TICKS(2100)); 
    }
}

void app_main() {
    printf("Inicializando Sistema Multitarea - Ejercicio 2\n");

    // Creación de tareas con diferentes prioridades (Mayor número = Mayor prioridad)
    xTaskCreate(vTaskSensor, "Task_Sensor", 2048, NULL, 1, NULL);
    xTaskCreate(vTaskLED,    "Task_LED",    2048, NULL, 2, NULL);
    xTaskCreate(vTaskSerial, "Task_Serial", 2048, NULL, 3, NULL);
}