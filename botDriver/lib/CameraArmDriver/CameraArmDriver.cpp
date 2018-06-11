#include <Arduino.h>
#include "CameraArmDriver.h"
#include <Servo.h>


CameraArmDriver::CameraArmDriver(){
  // DEFAULT
  Serial.begin(9600);
  Serial.println("hello");
}
void CameraArmDriver::begin(int basePin, int shoulderPin){
  _basePin = basePin;
  _shoulderPin = shoulderPin;

  _base.attach(_basePin,BASE_MIN_PW,BASE_MAX_PW);
  _shoulder.attach(_shoulderPin,BASE_MIN_PW,BASE_MAX_PW);
}

void CameraArmDriver::end(){
  _base.detach();
  _shoulder.detach();
}

void CameraArmDriver::turnBase(int angle){

  //TODO: added definition for function
  if(angle > currentBasePos){
    for(int i = currentBasePos;i < angle - S_OFFSET;i++){
      _base.write(i);
      delay(S_SPEED);
    }
  }else if(currentBasePos > angle){
    for(int i = currentBasePos;i > angle + S_OFFSET;i--){
      _base.write(i);
      delay(S_SPEED);
    }
  }
  //update global shoulder angle
  currentBasePos = angle;
}

void CameraArmDriver::turnShoulder(int angle){
  if(angle > currentShoulderPos){
    for(int i = currentShoulderPos;i < angle - S_OFFSET;i++){
      _shoulder.write(i);
      delay(S_SPEED);
    }
  }else if(currentShoulderPos > angle){
    for(int i = currentBasePos;i > angle + S_OFFSET;i--){
      _shoulder.write(i);
      delay(S_SPEED);
    }
  }
  // update global shouder angle
  currentShoulderPos = angle;
}


void CameraArmDriver::sweepBase(int count){
	int i =0;
	while(i < count){
		for(int pos = 0; pos < 180; pos++){
			_base.write(pos);
			delay(S_SPEED);
		}
		for(int pos = 180; pos > 0; pos--){
			_base.write(pos);
			delay(S_SPEED);
		}
		i++; //iteratte counter
	}
  
}

int CameraArmDriver::getCurrentBaseAngle(){
  return _base.read();
}

int CameraArmDriver::getCurrentShoulderAngle(){
  return currentShoulderPos;
}
