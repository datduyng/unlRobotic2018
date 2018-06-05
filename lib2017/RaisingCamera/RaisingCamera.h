/*
Author:	CheeTown Liew
Version of June 10, 2017

This algorithm is designed to drive the robot and run with precise displacement
using motor encoder, the algorithm includes simple proportion controller and 
displacement-encoder count convertor, speed comparator and path tracking.

Micro-controller Board: Arduino Mega
Motor Driver: Sabertooth Motor Driver
 
*/

#ifndef RaisingCamera_h
#define RaisingCamera_h

#include <Arduino.h>
#include <SabertoothSimplified.h>

//define constant
extern const double COUNTERPERREVOLUTION;

//define default encoder pins
#define EC3A    21 //LEFT encoder channel A
#define EC3B    22 //RIGHT encoder channel B
#define EGNDL   23  //LEFT encoder Vcc

//define default motor driver communication

//declare encoder counter

extern volatile long EC3;
extern volatile long EC3M;

//private functions
static void counter3(void);

//public functions--------------------------------------------------------
void cinit(void);
void raising(long); 
void raising2(long);
int A2C(int);
void downRGB(void);
void upRGB(void);



#endif

