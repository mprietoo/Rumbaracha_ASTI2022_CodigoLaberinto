# Rumbaracha_ASTI2022

Codigo del robot Rumbaracha, participante del concurso ASTI Challenge 2022

---------------- Pasos para hacer que el codigo compile -------------

1. Instalar la esp32 en el administrador de placas de arduino (IMPORTANTE: tener instalada version de escritorio de Arduino)

    En archivo -> preferencias -> gestor de URLs adicionales, añadir
    los siguientes links: 

    https://arduino.esp8266.com/stable/package_esp8266com_index.json
    https://dl.espressif.com/dl/package_esp32_index.json
    https://resource.heltec.cn/download/package_heltec_esp32_index.json

    Despues en herramientas -> placa -> gestor de tarjetas,
    buscar "ESP32" y "ESP8266" e instalar respectivamente.

2. Instalar las siguientes librerias.

    En arduino -> Añadir libreria  -> Administrador de bibliotecas, buscar e instalar las siguientes librerias:

    "ESPUI"
    "Adafruit_SSD1306" (Pedira instalar algunas librerias adicionales, decir que si).
    "Adafruit_VL5310X"
    "Adafruit_PWMServoDriver"
    "ESPUI" (Importante!! buscar en google el github de espui e instalar todo lo que pone en el apartado correspondiente).

3. Clonar el repositorio en visual code (o editor de preferencia) a una carpeta con el mismo nombre que el fichero .ino

---------------- Tutoriales utiles -------------

https://randomnerdtutorials.com/esp32-dual-core-arduino-ide/

https://adafruit.github.io/Adafruit_SSD1306/html/class_adafruit___s_s_d1306.html

https://github.com/s00500/ESPUI

https://www.luisllamas.es/como-controlar-hasta-16-gpio-por-i2c-con-extensor-pcf8575-y-arduino/


---------------- Repos utiles ------------------

https://github.com/adri5104/Codigo_pullup



