/************************************************************************************************
 * Programa: Pruebas de desarrollo para el control de actuadores en control digital
 * 
 * Descripción: Se establece la comunicación digital con el led incorporado de la tarjeta ESP32, la cual se 
 * conectada en el pin GPIO27, presentando un estado de conmutación de encendido y apagado, con una 
 * temporización de un segundo, adicionalmente de la presentación de su estado en terminal serial.
 *
 * Director: 
 *   - David Ricardo Rivera Albeladez
 * 
 * Co-director:
 *   - Victor Manuel Patiño Delgado
 * 
 * Autores:
 *   - Mariana Rivera Garcia
 *   - Juliana Muriel Barreto
 * 
 * Fecha de desarrollo: Agosto de 2023
 * 
 * Licencia: THE BEER-WARE LICENSE
 * As long as you retain this notice you can do whatever you want with this stuff. 
 * If we meet some day, and you think this stuff is worth it, you can buy me a beer in return.
************************************************************************************************/

/* Bibliotecas y declaraciones de preprocesador */
#include <stdio.h>                      // Incluye la biblioteca estándar de entrada y salida.
#include <stdlib.h>                     // Incluye la biblioteca estándar de funciones de propósito general.
#include "string.h"                     // Incluye la biblioteca para manipulación de cadenas de caracteres.
#include "freertos/FreeRTOS.h"          // Incluye la biblioteca FreeRTOS para programación multitarea en tiempo real.
#include "freertos/task.h"              // Incluye la biblioteca FreeRTOS para la administración de tareas.
#include "driver/gpio.h"                // Incluye la biblioteca para controlar pines GPIO en el ESP32.

/* Definición de constantes y macros */
#define LED_PIN 27                       // Define el número de pin para el LED.
uint8_t led_state = 0;                  // Variable que almacena el estado actual del LED (encendido o apagado).

/* Declaraciones de funciones (prototipos) */
esp_err_t init_led(void);               // Prototipo de la función para inicializar el LED.
esp_err_t toggle_led(void);             // Prototipo de la función para alternar el estado del LED.

/* Estructuras de datos y tipos personalizados */

/* Función principal (main) */
void app_main() {
    /* Declaración de variables locales */

    /* Inicialización y asignaciones */
    ESP_ERROR_CHECK(init_led());        // Inicializa el LED y verifica si hay errores.

    /* Estructura de control - Bucle(s) o condicionales */
    while (1) {                                                             // Bucle infinito.
        toggle_led();                                                       // Alterna el estado del LED.
        printf("LED estado: %s\n", led_state ? "Encendido" : "Apagado");    // Imprime el estado actual del LED.
        vTaskDelay(pdMS_TO_TICKS(1000));                                    // Pausa la tarea actual durante 1000 milisegundos.
    }
    
    /* Llamadas a funciones */
    /* Operaciones y cálculos */
    /* Entrada y salida de datos */
    /* Liberación de memoria */
    /* Retorno de valores y finalización del programa */
}

/* Implementación de funciones */
esp_err_t init_led(void)                            // Función para inicializar el LED.
{
    gpio_reset_pin(LED_PIN);                        // Resetea el pin GPIO utilizado por el LED.
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);  // Establece el pin GPIO como salida.
    return ESP_OK;                                  // Retorna ESP_OK para indicar que la inicialización fue exitosa.
}

esp_err_t toggle_led(void)              // Función para alternar el estado del LED.
{
    led_state = !led_state;             // Invierte el estado actual del LED.
    gpio_set_level(LED_PIN, led_state); // Establece el nivel del pin GPIO para controlar el LED.
    return ESP_OK;                      // Retorna ESP_OK para indicar que el cambio de estado fue exitoso.
}