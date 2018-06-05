/**
 * Author Dat Nguyen
 * Date: 6/4/2018
 *
 * This library help perform sequences and movement of the vamera Arm
 */


#ifndef CameraArmDriver_h
#define CameraArmDriver_h

#include <Servo.h>
#include <Arduino.h>



class CameraArmDriver{
  #define S_OFFSET 10
  #define S_SPEED 20 // um pwm delay interpret as speed.
  public:
    CameraArmDriver(Servo base, Servo shoulder);


    // init Servo
    void begin(void);
    void calibrate(void);
    void turnBase(int angle);
    int getCurrentXYAngle();
    int getCurrentXZAngle();

  private:
    Servo _base, _shoulder;
    int _basePin, _shoulderPin;
    int currentBasePos;
    int currentShoulderPos;
};


#endif
