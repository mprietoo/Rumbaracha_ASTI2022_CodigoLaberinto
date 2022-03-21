#ifndef _NAVCUADRICULA_H_
#define _NAVCUADRICULA_H_

#include "Config.h"
#include <QTRSensors.h>
#include "Motor.h"

#define HORARIO 1
#define ANTIHORARIO 0

extern const uint16_t THRESHOLD;
extern const float center;

//Clase que gestiona la navegacion en la prueba de la cuadricula
class NavCuadricula
{
    private:
        //Sensor siguelineas 
        QTRSensors* myQtr;
        uint16_t posicion;
        uint16_t sensorValues[6];
        float output;

        //Array con los dos motores a controlar
        Motor* MisMotores[2];

        //Pid
        PID::PIDController<float>* myPID ;
        PID::PIDParameters<float> parametros_PID;
        
        //Parametros de la velocidad
        uint8_t vel_base; 
        uint8_t vel_max;
        uint8_t vel_pid;

    public:

        //Constructor
        NavCuadricula(QTRSensors*, Motor*, Motor*);    
        void setVelBase(uint8_t a){vel_base = a;};
        void setPIDparam(PID::PIDParameters<float> a)
            {
                parametros_PID = a;
                myPID->SetTunings(a);
                };
        void parar();
        void avanzar();
        void girar(bool);
        void retroceder();
        void seguirLinea();
        bool sobreLineaHorizontal();
        void compute();
        float getOutput();
        float getPos(){return myPID->Input;};
        float getKp(){return myPID->GetKp();};
        float getKd(){return myPID->GetKd();};
        float getKi(){return myPID->GetKi();};
        void giro90(bool);
};

#endif