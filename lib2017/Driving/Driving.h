/*
Author:	CheeTown Liew
Version of June 10, 2017

This algorithm is designed to drive the robot and run with precise displacement
using motor encoder, the algorithm includes simple proportion controller and 
displacement-encoder count convertor, speed comparator and path tracking.

Micro-controller Board: Arduino Mega
Motor Driver: Sabertooth Motor Driver
 
*/

#ifndef Driving_h
#define Driving_h

#include <Arduino.h>
#include <SabertoothSimplified.h>


//define constant
#define ENCODERREVOLUTION 227.1 * 48
#define INCH2C 227.1 * 48 / ( 3 * 3.141592653589 * 2 * 2 )
#define COUNTER2INCHE ( 3 * 3.14159265359 * 2 * 2) / (227.1 * 48)
#define WHEELWIDTH 11.6875

//define default encoder pins
#define ECLA    2 //LEFT encoder channel A
#define ECRA    3 //RIGHT encoder channel A
#define ECLB    4 //LEFT encoder channel B
#define ECRB    5 //RIGHT encoder channel B
#define EPOWERL   52  //LEFT encoder Vcc
#define EPOWERR   53  //RIGHT encoder Vcc

//define default motor driver communication
#define MOTOR_DRIVER  14    //Using Tx_3 pin (Serial3)

//define controller constant
#define K1    0.440945		//LEFT controller constant
#define K2    0.5			//RIGHT controller constant
#define V	1.3			//speed controller constant
#define I	0.7		//integral controller constant

//declare global coordinates
extern float global_x;
extern float global_y;
extern float local_x;
extern float local_y;
extern int16_t global_orientation;

//declare encoder counter
extern volatile int64_t ECL;    //encoder counter - LEFT
extern volatile int64_t ECLM;  //encoder counter - LEFT MEMORY
extern volatile int64_t ECR;    //encoder counter - RIGHT
extern volatile int64_t ECRM;  //encoder counter - RIGHT MEMORY

//declare parameters
extern bool debug1;

//private functions
static void counter1(void);
static void counter2(void);

//public functions--------------------------------------------------------
void dinit (void);		//default initiator, needed before using the functions from this library
//void dinit ( uint8_t& pins );
void driveto( float distance );
void steer(int16_t toAngle );

void debugMode(void);
void debug(bool);


//converter functions
int64_t D2C( float distance );
float C2D (int64_t encoderCount);
int64_t R2C ( int16_t angle );
int16_t C2R ( int64_t encoderCount );

#endif

