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

  //define range of motor
  // these defineition need to be config and change everytime using a new set of servo.
  	// custom min/max pulse width settings that best suit my servos
  //shoulder
  const int CENTER_SHOULDER= 85;
  const int CW_SHOULDER= 0; // right
const int CCW_SHOULDER =180; // left
  const int SHOULDER_MIN_PW= 570;//calibrate the servo to get the best fit value.
  const int SHOULDER_MAX_PW =2400;

  //base
  const int CENTER_BASE =90;
  const int CW_BASE =0; // right
  const int CCW_BASE =180; // left
  const int BASE_MIN_PW =600;//calibrate the servo to get the best fit value.
  const int BASE_MAX_PW =2400;

	#define S_OFFSET 0
  #define S_SPEED 20 // um pwm delay interpret as speed.


    CameraArmDriver();// DEFAULT
    // init Servo
    void begin(int basePin, int shoulderPin);
    void end();
    void turnBase(char pos );
    void turnShoulder(char pos );
	void commandMode();

  private:
    Servo _base, _shoulder;
    int _basePin, _shoulderPin;
};


#endif
