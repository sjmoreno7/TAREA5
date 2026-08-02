# Tarea 5: ESP32 - UART, I2C y FreeRTOS
**Autor:** [Stefano Moreno]  


## 📌 Descripción del Proyecto
Este repositorio contiene la solución a la Tarea #5, enfocada en el desarrollo de aplicaciones para el microcontrolador **ESP32** utilizando el framework **ESP-IDF** bajo **PlatformIO**.

El proyecto abarca tres etapas de desarrollo:
1. **Ejercicio 1:** Implementación de un intérprete de comandos por puerto serial UART2.
2. **Ejercicio 2:** Estructuración del sistema serial utilizando arquitectura multitarea con **FreeRTOS**.
3. **Ejercicio 3 (Integración):** Comunicación entre tareas mediante **Colas (Queues)** de FreeRTOS, control de periféricos (UART2 y LED) y representación gráfica en tiempo real de los diagramas de tiempo en una pantalla **OLED SSD1306 (I2C)** como un osciloscopio lógico.


## 📁 Estructura del Código Fuente
- `src/ejercicio1.c`: Código fuente del intérprete de comandos UART2 en formato secuencial.
- `src/ejercicio2.c`: Implementación multitarea simple en FreeRTOS.
- `src/main.c`: Sistema completo integrado (UART2 + FreeRTOS Queues + I2C OLED).


## 🛠️ Requisitos e Instalación
- **Entorno de Desarrollo:** Visual Studio Code con la extensión PlatformIO.
- **Framework:** ESP-IDF (C).
- **Hardware utilizado:**
  - Microcontrolador ESP32 DevKit v1.
  - Pantalla OLED SSD1306 (128x64 pixels, protocolo I2C).
  - LED externo e interfaz UART-USB.

### Diagrama de Conexiones
| Periférico | Pin ESP32 | Protocolo / Función |
| :--- | :--- | :--- |
| **OLED SDA** | GPIO 21 | I2C Data |
| **OLED SCL** | GPIO 22 | I2C Clock |
| **UART TX** | GPIO 1 | UART2 Transmisión |
| **UART RX** | GPIO 3 | UART2 Recepción |
| **LED** | GPIO 19 | Salida Digital |

## 🚀 Comandos Seriales Disponibles
Conéctate mediante una terminal serial (115200 baudios, 8N1) y envía los siguientes comandos:
- `status`: Muestra el estado actual del sistema.
- `led on`: Enciende el LED conectado al GPIO19.
- `led off`: Apaga el LED conectado al GPIO19.
- `info`: Retorna los baudios configurados y el contador de comandos.
- `reset`: Reinicia los contadores internos del sistema.


## 📹 Demostración en Video
Puedes ver el video explicativo y la demostración en vivo del funcionamiento del osciloscopio en el siguiente enlace:  
👉 **[Ver Video en YouTube](https://youtu.be/ceQf1LIEL78)**
