#include <Arduino.h>
#include "CameraArmDriver.h"
#include <Servo.h>


void CameraArmDriver::begin(int basePin, int shoulderPin){
  _basePin = basePin;
  _shoulderPin = shoulderPin;

  _base.attach(_basePin);
  _shoulder.attach(_shoulderPin);
}

void CameraArmDriver::end(){
  _base.detach();
  _shoulder.detach();
}

void CameraArmDriver::turnBase(int angle){

  //TODO: added definition for function
  if(angle > currentBasePos){
    for(int i = currentBasePos;i < angle - S_OFFSET;i++){
      Servo.write(i);
      delay(S_SPEED);
    }
  }else if(currentBasePos > angle){
    for(int i = currentBasePos;i > angle + S_OFFSET;i--){
      Servo.write(i);
      delay(S_SPEED);
    }
  }
  //update global shoulder angle
  currentBasePos = angle;
}

void CameraArmDriver::turnShoulder(int angle){
  if(angle > currentShoulderPos){
    for(int i = currentShoulderPos;i < angle - S_OFFSET;i++){
      Servo.write(i);
      delay(S_SPEED);
    }
  }else if(currentShoulderPos > angle){
    for(int i = currentBasePos;i > angle + S_OFFSET;i--){
      Servo.write(i);
      delay(S_SPEED);
    }
  }
  // update global shouder angle
  currentShoulderPos = angle;
}

int CameraArmDriver::getCurrentBaseAngle(){
  return currentBasePos;
}

int CameraArmDriver::getCurrentShoulderAngle(){
  return currentShoulderPos;
}
