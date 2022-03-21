#ifdef _A_

#include <QTRSensors.h>


// This example is designed for use with eight RC QTR sensors. These
// reflectance sensors should be connected to digital pins 3 to 10. The
// sensors' emitter control pin (CTRL or LEDON) can optionally be connected to
// digital pin 2, or you can leave it disconnected and remove the call to
// setEmitterPin().
//
// The setup phase of this example calibrates the sensors for ten seconds and
// turns on the Arduino's LED (usually on pin 13) while calibration is going
// on. During this phase, you should expose each reflectance sensor to the
// lightest and darkest readings they will encounter. For example, if you are
// making a line follower, you should slide the sensors across the line during
// the calibration phase so that each sensor can get a reading of how dark the
// line is and how light the ground is.  Improper calibration will result in
// poor readings.
//
// The main loop of the example reads the calibrated sensor values and uses
// them to estimate the position of a line. You can test this by taping a piece
// of 3/4" black electrical tape to a piece of white paper and sliding the
// sensor across it. It prints the sensor values to the serial monitor as
// numbers from 0 (maximum reflectance) to 1000 (minimum reflectance) followed
// by the estimated location of the line as a number from 0 to 5000. 1000 means
// the line is directly under sensor 1, 2000 means directly under sensor 2,
// etc. 0 means the line is directly under sensor 0 or was last seen by sensor
// 0 before being lost. 5000 means the line is directly under sensor 5 or was
// last seen by sensor 5 before being lost.

QTRSensors qtr;

const uint8_t SensorCount = 6;
uint16_t sensorValues[SensorCount];

// Motor A
int ENA = 2;
int IN1 = 22;
int IN2 = 23;

// Motor B
int ENB = 3;
int IN3 = 24;
int IN4 = 25;

int count;

const int offsetA = 1;
const int offsetB = 1;

float error_anterior = 0.0;
float integral = 0.0;
float error=0.0;

//Velocidades
#define VEL_BASE 140
#define VEL_MAX 255

//PID Constantes
#define Kp 1.1 //0.05
#define Kd 2
#define Ki 0

#define threshold 4500
 //Los 6 sensores detectando negro
#define timesD 0 //Número de rayas necesarias en las que gira a la derecha
#define timesI 0 //Número de rayas necesarias en las que gira a la izquierda
#define timesDI 2 //Número de rayas necesarias en las que gira a la derecha, una vez ha girado a la izquierda
#define timesID 0 //Número de rayas necesarias en las que gira a la izquierda, una vez ha girado a la izquierda
const float center=2500.0;

//Delays
#define timestop 200
#define timegiroD 550
#define timegiroI 550
#define timego 100


void setup()
{

  // Motor A
int ENA = 2;
int IN1 = 22;
int IN2 = 23;

// Motor B
int ENB = 3;
int IN3 = 24;
int IN4 = 25;


  // configure the sensors
  qtr.setTypeRC();
  qtr.setSensorPins((const uint8_t[]) {
    A0, A1, A2, A3, A4, A5
  }, SensorCount);
  //qtr.setEmitterPin(2);

  delay(500);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH); // turn on Arduino's LED to indicate we are in calibration mode

  // 2.5 ms RC read timeout (default) * 10 reads per calibrate() call
  // = ~25 ms per calibrate() call.
  // Call calibrate() 400 times to make calibration take about 10 seconds.
  for (uint16_t i = 0; i < 50; i++)
  {
    qtr.calibrate();
  }
  digitalWrite(LED_BUILTIN, LOW); // turn off Arduino's LED to indicate we are through with calibration

  // print the calibration minimum values measured when emitters were on
  Serial.begin(9600);
  for (uint8_t i = 0; i < SensorCount; i++)
  {
    Serial.print(qtr.calibrationOn.minimum[i]);
    Serial.print(' ');
  }
  Serial.println();

  // print the calibration maximum values measured when emitters were on
  for (uint8_t i = 0; i < SensorCount; i++)
  {
    Serial.print(qtr.calibrationOn.maximum[i]);
    Serial.print(' ');
  }
  Serial.println();
  Serial.println();
  delay(1000);

}


void loop()
{
  // read calibrated sensor values and obtain a measure of the line position
  // from 0 to 5000 (for a white line, use readLineWhite() instead)
  uint16_t position = qtr.readLineBlack(sensorValues);

  // print the sensor values as numbers from 0 to 1000, where 0 means maximum
  // reflectance and 1000 means minimum reflectance, followed by the line
  // position
  for (uint8_t i = 0; i < SensorCount; i++)
  {
    Serial.print(sensorValues[i]);
    Serial.print('\t');
   
  }
  Serial.print(position);
  Serial.print('\n');


  //Velocidades del motor A y B
  float vel_a;
  float vel_b;

  //Sensores
  float SensorTotal;


  error=(position - center)/10;
  integral = integral + error;
  if (position*integral<0) integral=0.0;
  vel_a = VEL_BASE - (Kp * error + Kd * (error - error_anterior) + Ki * integral);
  vel_b = VEL_BASE + Kp * error + Kd * (error - error_anterior) + Ki * integral;

  error_anterior = error;

  int suma=0;
  int i;
  for(i=0;i<6;i++){
    suma+=sensorValues[i];
  }
  
  if (suma>threshold){

     if (timesD!=0){
      count++;
      if (count == timesD){
        Parar();
        delay(timestop);
        Derecha();
        delay(timegiroD);
      }
      else{
        Adelante(VEL_BASE,VEL_BASE);
        delay(timego);
      }
     }
     
     if (timesDI!=0){
      if (!count){
        Parar();
        delay(timestop);
        Derecha();
        delay(timegiroD);
        count++;
      }
      else if (count == timesDI){
        Parar();
        delay(timestop);
        Izquierda();
        delay(timegiroD);
        count++;
      }
      else{
        Adelante(VEL_BASE,VEL_BASE);
        delay(timego);
        count++;
      }
     }
     
    if (timesI!=0){
      count++;
      if (count == timesI){
        Parar();
        delay(timestop);
        Izquierda();
        delay(timegiroI);
      }
      else{
        Adelante(VEL_BASE,VEL_BASE);
        delay(timego);
      }
     }
      
     if (timesID!=0){
      if (!count){
        Parar();
        delay(timestop);
        Izquierda();
        delay(timegiroI);
        count++;
      }
      else if (count == timesID){
        Parar();
        delay(timestop);
        Derecha();
        delay(timegiroD);
        count++;
      }
      else{
        Adelante(VEL_BASE,VEL_BASE);
        delay(timego);
        count++;
     }
    }
    Serial.print(count);
    Serial.print('\n');
    } 
    else{
    Serial.print('B');
    }  
//
  

  if (vel_a>VEL_MAX) vel_a=VEL_MAX;
  if (vel_b>VEL_MAX) vel_b=VEL_MAX;
  if (vel_a<0) vel_a=0;
  if (vel_b<0) vel_b=0;
  
   Adelante(vel_a, vel_b);
     Serial.print('\n');
  Serial.print(error);
    Serial.print('\n');
  Serial.print(vel_a);
    Serial.print('\n');
  Serial.print(vel_b);
    Serial.print('\n');
},


void Derecha ()
{
 //Direccion motor A
 digitalWrite (IN1, HIGH);
 digitalWrite (IN2, LOW);
 analogWrite (ENA, 200); //Velocidad motor A
 //Direccion motor B
 digitalWrite (IN3, LOW);
 digitalWrite (IN4, HIGH);
 analogWrite (ENB, 100); //Velocidad motor A
}

void Izquierda ()
{
 //Direccion motor A
 digitalWrite (IN1, LOW);
 digitalWrite (IN2, HIGH);
 analogWrite (ENA, 50); //Velocidad motor A
 //Direccion motor B
 digitalWrite (IN3, HIGH);
 digitalWrite (IN4, LOW);
 analogWrite (ENB, 150); //Velocidad motor A
}
void Parar ()
{
 //Direccion motor A
 digitalWrite (IN1, LOW);
 digitalWrite (IN2, LOW);
 analogWrite (ENA, 0); //Velocidad motor A
 //Direccion motor B
 digitalWrite (IN3, LOW);
 digitalWrite (IN4, LOW);
 analogWrite (ENB, 0); //Velocidad motor A
}
void Adelante (float vel_a, float vel_b)
{
 //Direccion motor A
 digitalWrite (IN1, HIGH);
 digitalWrite (IN2, LOW);
 analogWrite (ENA,vel_a ); //Velocidad motor A
 //Direccion motor B
 digitalWrite (IN3, HIGH);
 digitalWrite (IN4, LOW);
 analogWrite (ENB, vel_b); //Velocidad motor B
}

void Atras (float vel_a, float vel_b)
{
 //Direccion motor A
 digitalWrite (IN1, LOW);
 digitalWrite (IN2, HIGH);
 analogWrite (ENA, vel_a); //Velocidad motor A
 //Direccion motor B
 digitalWrite (IN3, LOW);
 digitalWrite (IN4, HIGH);
 analogWrite (ENB, vel_b); //Velocidad motor B
}









#endif