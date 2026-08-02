#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "driver/gpio.h"

#define UART_PORT_NUM      UART_NUM_2
#define UART_BAUD_RATE     115200
#define BUF_SIZE           1024


#define UART_TX_PIN        1
#define UART_RX_PIN        3
#define LED_PIN            GPIO_NUM_21

int command_counter = 0;
char system_status[50] = "Operativo";

void init_hardware() {
    gpio_reset_pin(LED_PIN);
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);
    gpio_set_level(LED_PIN, 0);

    uart_config_t uart_config = {
        .baud_rate = UART_BAUD_RATE,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB,
    };

    uart_driver_install(UART_PORT_NUM, BUF_SIZE * 2, 0, 0, NULL, 0);
    uart_param_config(UART_PORT_NUM, &uart_config);
    uart_set_pin(UART_PORT_NUM, UART_TX_PIN, UART_RX_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
}

void send_response(const char* response) {
    uart_write_bytes(UART_PORT_NUM, response, strlen(response));
    uart_write_bytes(UART_PORT_NUM, "\r\n", 2);
}

void process_command(char* cmd) {
    command_counter++;
    cmd[strcspn(cmd, "\r\n")] = 0; 

    if (strcmp(cmd, "status") == 0) {
        char res[128]; // <-- Ajustado a 128 bytes para evitar el error de truncamiento
        snprintf(res, sizeof(res), "Estado del sistema: %s", system_status);
        send_response(res);
    } 
    else if (strcmp(cmd, "led on") == 0) {
        gpio_set_level(LED_PIN, 1);
        send_response("Accion: LED encendido");
    } 
    else if (strcmp(cmd, "led off") == 0) {
        gpio_set_level(LED_PIN, 0);
        send_response("Accion: LED apagado");
    } 
    else if (strcmp(cmd, "info") == 0) {
        char res[128];
        snprintf(res, sizeof(res), "Info -> Baud rate: %d, Puerto: UART2, Comandos procesados: %d", UART_BAUD_RATE, command_counter);
        send_response(res);
    } 
    else if (strcmp(cmd, "reset") == 0) {
        command_counter = 0;
        strcpy(system_status, "Operativo (Reiniciado)");
        gpio_set_level(LED_PIN, 0);
        send_response("Variables internas reiniciadas.");
    } 
    else if (strlen(cmd) > 0) {
        send_response("Error: Comando no reconocido.");
    }
}

void app_main(void) {
    init_hardware();
    
    uint8_t* data = (uint8_t*) malloc(BUF_SIZE);
    char rx_buffer[256];
    int rx_idx = 0;

    send_response("Sistema inicializado. Ingrese un comando (status, led on, led off, info, reset):");

    while (1) {
        int len = uart_read_bytes(UART_PORT_NUM, data, (BUF_SIZE - 1), pdMS_TO_TICKS(20));
        
        if (len > 0) {
            for (int i = 0; i < len; i++) {
                char c = (char)data[i];
                uart_write_bytes(UART_PORT_NUM, &c, 1);
                
                if (c == '\n' || c == '\r') {
                    if (rx_idx > 0) {
                        rx_buffer[rx_idx] = '\0';
                        send_response(""); 
                        process_command(rx_buffer);
                        rx_idx = 0;
                    }
                } else {
                    if (rx_idx < sizeof(rx_buffer) - 1) {
                        rx_buffer[rx_idx++] = c;
                    }
                }
            }
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }
    free(data);
}