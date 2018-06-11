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
   
  public:
  
	#define S_OFFSET 0
    #define S_SPEED 20 // um pwm delay interpret as speed.
	
	// custom min/max pulse width settings that best suit my servos

	//calibrate the servo to get the best fit value.
	#define BASE_MIN_PW 600
	#define BASE_MAX_PW 2320
	#define SHOULDER 600
	#define SHOULDER 2320
	
    CameraArmDriver();// DEFAULT
    // init Servo
    void begin(int basePin, int shoulderPin);
    void end();
    void calibrate(void);
    void turnBase(int angle);
    void turnShoulder(int angle);
    int getCurrentBaseAngle();
    int getCurrentShoulderAngle();
	void sweepBase(int count);

  private:
    Servo _base, _shoulder;
    int _basePin, _shoulderPin;
    int currentBasePos;
    int currentShoulderPos;
};


#endif
