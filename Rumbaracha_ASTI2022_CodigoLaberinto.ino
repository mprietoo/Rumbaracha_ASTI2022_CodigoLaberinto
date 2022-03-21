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

 //Objeto global para el extensor de pines
PCF8575 pinExtensor =  PCF8575(0x20);

//objeto global para el display
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET); 

//Objeto que maneja los 3 sensores tof
TofSensors MySensors(1, 2, 3, &pinExtensor); 

//Objetos para los dos motores (NO control de posicion)
Motor Motor_derecho(PIN_MOTOR_D_IN1, PIN_MOTOR_D_IN2, PIN_MOTOR_D_PWM, PWM_CH_D, PWM_FREC, PWM_RES); //Motor derecho
Motor Motor_izquierdo(PIN_MOTOR_I_IN1, PIN_MOTOR_I_IN2, PIN_MOTOR_I_PWM, PWM_CH_I, PWM_FREC, PWM_RES); //Motor izquierdo

void setup(){

}

void loop(){
    
}