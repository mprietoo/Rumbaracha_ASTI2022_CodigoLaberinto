#ifndef CONFIG_
#define CONFIG_

#include "funcionesSetup.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "TofSensors.h"
#include "funcionesSetup.h"
#include "PCF8575.h"
#include "Pinout.h"


#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

#define TOF_ADRESS_FRONT 0x30
#define TOF_ADRESS_LEFT 0x31
#define TOF_ADRESS_RIGHT 0x32

#define FRONT 2
#define RIGHT 0
#define LEFT 1
#define LED_BUILTIN 2


// --------------- Movidas para la salida pwm --------------
#define PWM_FREC 5000 //frecuencia de la senal pwm
#define PWM_RES 8// resolucion de la senal pwm. En nuestro caso 8 bits (0 - 255)
#define PWM_CH_D 0 //canal para el pwm del motor derecho 
#define PWM_CH_I 1 //canal para el pwm del motor izquierdo


#endif