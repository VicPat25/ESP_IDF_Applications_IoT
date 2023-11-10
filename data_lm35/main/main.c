/************************************************************************************************
 * Programa: Pruebas de desarrollo para el control de sensores analógicos
 * 
 * Descripción: Programa diseñado para leer datos de temperatura del sensor LM35 mediante el 
 * microcontrolador ESP32; mediante la confuguración y calibración del convertidor analógico-digital (ADC),
 * se adquieren señales del sensor LM35, las cuales se traducen en valores de temperatura en grados Celsius. 
 * Estos valores son posteriormente impresos en la consola con dos decimales.
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
#include "freertos/FreeRTOS.h"          // Incluye la biblioteca FreeRTOS para programación multitarea en tiempo real.
#include "freertos/task.h"              // Incluye la biblioteca FreeRTOS para la administración de tareas.
#include "driver/adc.h"                 // Incluye la biblioteca de control analógo digitales
#include "esp_adc_cal.h"                // Incluye la biblioteca para los cálculos de los sistenas analógos digitales

/* Definición de constantes y macros */
#define LM35_GPIO_PIN ADC1_CHANNEL_7  // Define el número de pin para el sensor LM35

#define VOLTAGE_SUPPLY 5000 // Definimos la tensión de alimentación del sensor LM35 (5V en este caso)

#define ADC_REF_VOLTAGE 1100 // Punto de referencia del ADC para obtener lecturas precisas (esto puede variar según el ESP32 utilizado)
#define ADC_WIDTH_BIT 12  //almacenamiento de memoria
#define ADC_MAX_VALUE ((1<<ADC_WIDTH_BIT)-1)  //corrimiento por etado de memoria

/* Declaraciones de funciones (prototipos) */
void lm35_task(void *pvParameters);

/* Estructuras de datos y tipos personalizados */

/* Función principal (main) */
void app_main() {

    /* Declaración de variables locales */

    /* Inicialización y asignaciones */
    adc1_config_width(ADC_WIDTH_BIT); // Configurar el ADC para el pin del sensor LM35
    adc1_config_channel_atten(LM35_GPIO_PIN, ADC_ATTEN_DB_11); // configuracion de canal

    xTaskCreate(lm35_task, "lm35_task", 2048, NULL, 3.3, NULL); // Crear tarea para leer el sensor LM35

    /* Estructura de control - Bucle(s) o condicionales */
    /* Llamadas a funciones */
    /* Operaciones y cálculos */
    /* Entrada y salida de datos */
    /* Liberación de memoria */
    /* Retorno de valores y finalización del programa */
}

/* Implementación de funciones */
void lm35_task(void *pvParameters) {
    esp_adc_cal_characteristics_t adc_chars; // habilitar las funciones especiales para calculos en variables tipo char
    esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT, ADC_REF_VOLTAGE, &adc_chars); //sistema de regresion lineal para la variable adquirida

    while (1) {
        uint32_t adc_reading = adc1_get_raw(LM35_GPIO_PIN); //lectura de datos
        uint32_t millivolts = esp_adc_cal_raw_to_voltage(adc_reading, &adc_chars); //reajuste de parametros

        float temperature = millivolts / 10.0; // Convertir el valor leído a grados Celsius según la sensibilidad del LM35 (10 mV/°C)

        printf("Temperatura: %.2f °C\n", temperature); //visualizacion en terminal serial
        vTaskDelay(pdMS_TO_TICKS(1000)); // Esperar 1 segundo antes de leer nuevamente
    }
}

