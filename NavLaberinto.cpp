#include "NavLaberinto.h"

NavLaberinto::NavLaberinto(Motor* b, Motor* c )
{
    
    MisMotores[RIGHT] = b;
    MisMotores[LEFT] = c;

    PID::PIDParameters<float> aux(1.0,0.0,0.0);
    parametros_PID = aux;
    myPID = new PID::PIDController<float>(aux);

    vel_max = 255;
    vel_base = 100;
    myPID->Setpoint = 0.0;
    myPID->SetOutputLimits(-255, 255);
    myPID->TurnOn();
    
}

void NavLaberinto::parar()
{
    int i;
    for(i = 0; i< 2; i++)
    {
        MisMotores[i]->setPWM(0);
        MisMotores[i]->setStop();
    }
}

void NavLaberinto::retroceder()
{
    MisMotores[RIGHT]->setBack();
    MisMotores[LEFT]->setBack();
    MisMotores[RIGHT]->setPWM(200);
    MisMotores[LEFT]->setPWM(200) ;
}

void NavLaberinto::avanzar()
{
    MisMotores[RIGHT]->setFwd();
    MisMotores[LEFT]->setFwd();
    MisMotores[RIGHT]->setPWM(200);
    MisMotores[LEFT]->setPWM(200) ;
}

void NavLaberinto::girar(bool sentido)
{
    if(sentido == HORARIO)
    {
        MisMotores[RIGHT]->setFwd();
        MisMotores[LEFT]->setBack();
        MisMotores[RIGHT]->setPWM(200);
        MisMotores[LEFT]->setPWM(200) ;
    }else
    {
        MisMotores[RIGHT]->setBack();
        MisMotores[LEFT]->setFwd();
        MisMotores[RIGHT]->setPWM(200);
        MisMotores[LEFT]->setPWM(200) ;
    }
}

void NavLaberinto::giro90(bool sentido)
{
    if(sentido == HORARIO)
    {
        MisMotores[RIGHT]->setFwd();
        MisMotores[LEFT]->setBack();
        MisMotores[RIGHT]->setPWM(200);
        MisMotores[LEFT]->setPWM(200) ;
    }else
    {
        MisMotores[RIGHT]->setBack();
        MisMotores[LEFT]->setFwd();
        MisMotores[RIGHT]->setPWM(200);
        MisMotores[LEFT]->setPWM(200) ;
    }

    delay(MILLIS_GIRO90); // puede que haya que poner uno en cada dirección
    this->parar();
}




