/************************************************************************************************
 * Programa: Pruebas de desarrollo para el control de display OLED ssd1306 mediante pulsador
 * 
 * Descripción: Programa diseñado para la configuración de un display OLED SSD1306, mediante la 
 * configuración de un sistema de comunicación I2C, en donde se establecen los pines GPIO22 y GPIO21
 * como SCL Y SDA respectivamente, se adiciona una pulsador que se establece en el pin GPIO25, cuando 
 * el esta de este sea alto se escribira el númeo 1 en el display OLED y cuando sea bajo el número 0
 * mediante las funciones que proporciona la libreria ssd1306.
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
 * Fecha de desarrollo: Septiembre de 2023
 * 
 * Licencia: THE BEER-WARE LICENSE
 * As long as you retain this notice you can do whatever you want with this stuff. 
 * If we meet some day, and you think this stuff is worth it, you can buy me a beer in return.
************************************************************************************************/

/* Bibliotecas y declaraciones de preprocesador */
#include <stdio.h>                  //Incluye la biblioteca estandar de entrada y salida
#include <stdlib.h>                 //Incluye la biblioteca para la gestión de memoria dinámica
#include "freertos/FreeRTOS.h"      // Incluye la biblioteca FreeRTOS para programación multitarea en tiempo real
#include "freertos/task.h"          // Incluye la biblioteca FreeRTOS para la administración de tareas
#include "ssd1306.h"                //Incluye la biblioteca que permite acceso a funciones del dispositivo SSD1306
#include "driver/gpio.h"            //Incluye la biblliteca que permite utilizar las capacidades de control de pines GPIO      

/* Definición de constantes y macros */
#define I2C_MASTER_SCL_IO 22        //Define el pin GPIO22 I2C como SCL
#define I2C_MASTER_SDA_IO 21        //Define el pin GPIO21 I2C como SDA
#define I2C_MASTER_NUM I2C_NUM_1    //Define el número de puerto I2C para el desarrollador maestro
#define I2C_MASTER_FREQ_HZ 100000   //Define la frecuebcia del reloj del maestro I2C
#define PIN_PULSADOR  GPIO_NUM_25   //Define el pin del pulsador en GPIO25

//Declaración de funciones
void configurar_pines();            //Define función que configura el pin del pulsador como un GPIO de entrada
int leer_pulsador();                //Define función que permite leer el estado del pulsador

//Declara variable estatica ssd1306_dev de tipo ssd1306_handle_t incicializada en NULL
static ssd1306_handle_t ssd1306_dev = NULL;


// Función principal (main) 
void app_main(void)
{   
    configurar_pines();

    //Configuración de la comunicación I2C
    i2c_config_t conf;
    //Define comunicación en modo maestro
    conf.mode = I2C_MODE_MASTER;
    //Define el pin SDA
    conf.sda_io_num = (gpio_num_t)I2C_MASTER_SDA_IO;
    //Habilita resistencia pull up del pin SDA
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    //Define el pin SCL
    conf.scl_io_num = (gpio_num_t)I2C_MASTER_SCL_IO;
    //Habilita resistencia pull up del pin SCL
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    //Define la frecuencia del reloj del maestro
    conf.master.clk_speed = I2C_MASTER_FREQ_HZ;
    //Establece  la señal de reloj (SCL) para I2C como normal
    conf.clk_flags = I2C_SCLK_SRC_FLAG_FOR_NOMAL;
    //Configura parametros del bus I2C
    i2c_param_config(I2C_MASTER_NUM, &conf);
    //Instala y configura el controlador del bus I2C
    i2c_driver_install(I2C_MASTER_NUM,conf.mode, 0, 0, 0);
    
    /*Configuración del dispositivo SSD1306 */
    //Crea instancia del dispositivo SSD1306 que se comunicará a través del bus I2C configurado
    ssd1306_dev = ssd1306_create(I2C_MASTER_NUM, SSD1306_I2C_ADDRESS);
    //Actualiza físicamente los píxeles en la pantalla OLED con la información almacenada en la memoria GRAM
    ssd1306_refresh_gram(ssd1306_dev);
    //Establece todos los pixeles de la OLED en negro, borra la pantalla
    ssd1306_clear_screen(ssd1306_dev, 0x00);

    //Se define un ciclo while para que se ejecute de forma indefinida
    while(1){
        //guardar en la variable el estado del pulsador
        int estado_pulsador = leer_pulsador();
        
        // Actualizar la pantalla OLED
        ssd1306_clear_screen(ssd1306_dev, 0x00);

        //Condicional que escribe 1 en la OLED si el estado del pulsador es alto y 0 si es bajo
        if(estado_pulsador==1){
            //Escribe 1 en la OLED
            ssd1306_draw_string(ssd1306_dev, 20, 10, (const uint8_t *)"1", 16, 1);
            
        }else{
            //Escribe 0 en la OlED
            ssd1306_draw_string(ssd1306_dev, 20, 10, (const uint8_t *)"0", 16, 1);
        }

        //refresca la pantalla
        ssd1306_refresh_gram(ssd1306_dev);
       
       //Delay de 500ms
        vTaskDelay(pdMS_TO_TICKS(500));

    }
     
}

//Configura el pin GPIO del pulsador como entrada
void configurar_pines() {
    //Configura pines GPIO
    gpio_config_t io_conf;
    //Deshabilita interrupciones del pin
    io_conf.intr_type = GPIO_INTR_DISABLE;
    //Define el pin como entrada
    io_conf.mode = GPIO_MODE_INPUT;
    //configura la máscara de bits del pin GPIO
    io_conf.pin_bit_mask = (1ULL << PIN_PULSADOR);
    //Habilita resistencia pull-up interna del pin GPIO
    io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
    //Aplica la configuración establecida al pin GPIO
    gpio_config(&io_conf);
}

//Función que permite leer el estado del pulsador
int leer_pulsador() {
    //retorna el estado del pin GPIO25
    return gpio_get_level(PIN_PULSADOR);
}
