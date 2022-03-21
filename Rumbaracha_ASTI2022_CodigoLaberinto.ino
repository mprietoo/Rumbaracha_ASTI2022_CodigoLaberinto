//Cosas que hacer:
/*
f1 -> Arduino: Board config -> ESP32 Dev Module
f1 -> Arduino: Initialize (solo una vez)
f1 -> Arduino: Verify (compilar)
*/

#include "funcionesSetup.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "TofSensors.h"
#include "funcionesSetup.h"
#include "PCF8575.h"
#include "Pinout.h"
#include "Config.h"
#include "Motor.h"
#include "NavLaberinto.h"
#include "Logo.h"


 //Objeto global para el extensor de pines
PCF8575 pinExtensor =  PCF8575(0x20);

//objeto global para el display
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET); 

//Objeto que maneja los 3 sensores tof
TofSensors MySensors(1, 2, 3, &pinExtensor); 

//Objetos para los dos motores (NO control de posicion)
Motor Motor_derecho(PIN_MOTOR_D_IN1, PIN_MOTOR_D_IN2, PIN_MOTOR_D_PWM, PWM_CH_D, PWM_FREC, PWM_RES); //Motor derecho
Motor Motor_izquierdo(PIN_MOTOR_I_IN1, PIN_MOTOR_I_IN2, PIN_MOTOR_I_PWM, PWM_CH_I, PWM_FREC, PWM_RES); //Motor izquierdo

// Objeto que maneja los motores
NavLaberinto misMotores(&Motor_derecho, &Motor_izquierdo);

// Movidas del multitasking
TaskHandle_t Task_CORE0; // Nucleo secundario
TaskHandle_t Task_CORE1; // Nucleo principal

void setup(){
Wire.setClock(800000);
    

    // Inicializamos el serial
    Serial.begin(9600);

    // Se inicializa y configura el display
    configDisplay();
    
    display.clearDisplay();
    display.drawBitmap(
    (display.width()  - LOGO_WIDTH ) / 2,
    (display.height() - LOGO_HEIGHT) / 2,
    logo_reset, LOGO_WIDTH, LOGO_HEIGHT, 1);
    display.display();
    delay(2000);

    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);
    display.setCursor(1, 1);
    display.println("Configurando Hardware...");
    display.display();

    Motor_derecho.init();
    Motor_izquierdo.init();

    
    // Se inicializan los pines de los tof
    MySensors.init();

    // Se inicializa el extensor de pines
    pinExtensor.begin();

    // Se cambia la direccion de memoria de los sensores tof y se bootean
    MySensors.setID(TOF_ADRESS_FRONT, TOF_ADRESS_RIGHT, TOF_ADRESS_LEFT);
    display.println(" Listo");
    display.display();

    display.println(" Listo");
    display.display();

    delay(1000);
    display.clearDisplay();

    // -------------- Creamos los dos hilos en los dos nucleos ----------------

    // Para el core 0 (secundario)
    xTaskCreatePinnedToCore(
        TaskCORE0code, // funcion de la task
        "Task_CORE0",  // nombre de la task
        10000,         // tamano del stack de la task
        NULL,          // Parametos
        1,             // task priority
        &Task_CORE0,   // callback
        0              // se lo mandamos al nucleo 0
    );

    // Para el core 1 (principal)
    xTaskCreatePinnedToCore(
        TaskCORE1code, // funcion de la task
        "Task_CORE1",  // nombre de la task
        10000,         // tamano del stack de la task
        NULL,          // Parametos
        1,             // task priority
        &Task_CORE1,   // callback
        1              // se lo mandamos al nucleo 1
    );
}

//------------------------- EL codigo  -------------------------------------------

// VARIABLES
//constantes:
#define t_rotacion_dech 800
#define t_rotacion_izq 600
#define t_atras 600
#define t_parar 100

float vel_dech;
float vel_izq;
int error;
int estado=0; // no sé qué es


int distanciaF;
int distanciaD;
int distanciaI;
int contador = 0; // solo para el inicio


// El core 0 lo usaremos para manejar la movida web
//  y alguna tarea secundaria
void TaskCORE0code(void *pvParameters)
{ 
    int lineas_a_contar = 0;
    bool flag_horizontal = false;
    uint8_t numero_de_veces_giradas = 0;
    Serial.println("Core 0 activo");
    display.println("Core 0 activo");
    // Bucle infinito // AQUÍ PROGRAMO YO
    for (;;)
    {
        // solo para entrar en el laberinto
        if(contador == 0){
            misMotores.avanzar();
            delay(2000); // que alguien cambie esta basura
        }

        // leo sensores
        distanciaF = MySensors.getDistance(FRONT);
        distanciaD = MySensors.getDistance(RIGHT);
        distanciaI = MySensors.getDistance(LEFT);

        // casuística:

        // recto
        misMotores.mantener_carril(); // hace el pid
        
        if ((distanciaF < 5)||(distanciaD < 5)||(distanciaI < 5)) {
            misMotores.parar();
            // Parar();
            // delay(t_parar);
            misMotores.retroceder();
            // Atras();
            // delay(t_atras);
        }


        if(estado){  //miro la pared derecha
        //giros
        if (distanciaD > 35) {  //hueco a la derecha abandono via
            misMotores.mantener_carril(); // hace el pid
            // Adelante(128, 128);
            // delay(t_parar);
            estado=!estado;
        }
    
        else if (distanciaF < 15) {   //cerca de la pared
            if (distanciaI > 35){
                misMotores.giro90(HORARIO); // gira a la izq // ¿HORARIO?
                    // Izquierda();
                    // delay(t_rotacion_izq);
            }
            else{
                misMotores.parar();
                // Parar();
                // delay(t_parar);
                misMotores.retroceder();
                // Atras();
                // delay(t_atras);
            }
        }

        else{
            misMotores.mantener_carril();
            // Adelante(vel_dech, vel_izq);
        }
        }

        if(!estado){  //miro la pared izquierda
        //giros
        if (distanciaI > 35) {  //hueco a la izquierda abandono via
            misMotores.mantener_carril();
            // Adelante(128, 128);
            // delay(t_parar);
            estado=!estado;
        }
            
        else if (distanciaF < 15) {  //cerca de la pared
            if (distanciaD > 35){
                    misMotores.giro90(ANTIHORARIO); // ANTIHORARIO???
                    // Derecha();
                    // delay(t_rotacion_dech);
            }
            else{
                misMotores.parar();
                // Parar();
                // delay(t_parar);
                misMotores.retroceder()
                // Atras();
                // delay(t_atras);
            }
        }

        else{
            misMotores.mantener_carril();
            // Adelante(vel_dech, vel_izq);
        }
    

       

    }
}

// El core 1 contiene cosas secundarias
void TaskCORE1code(void *pvParameters)
{
    Serial.println("Core 1 activo");
    display.println("Core 1 activo");
    // Bucle infinito // AQUÍ PROGRAMO YO (wifi)
    for (;;)
    {
        
        
        //vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

// No usado
void loop(void)
{
}