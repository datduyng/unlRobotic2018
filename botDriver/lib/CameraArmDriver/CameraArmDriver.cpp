#include <Arduino.h>
#include "CameraArmDriver.h"
#include <Servo.h>


CameraArmDriver::CameraArmDriver(){
  // DEFAULT
}
void CameraArmDriver::begin(int basePin, int shoulderPin){
  _basePin = basePin;
  _shoulderPin = shoulderPin;

  _base.attach(_basePin,BASE_MIN_PW,BASE_MAX_PW);
  _shoulder.attach(_shoulderPin,SHOULDER_MIN_PW,SHOULDER_MAX_PW);

  Serial.begin(115200);
}

void CameraArmDriver::end(){
  _base.detach();
  _shoulder.detach();
}

void CameraArmDriver::turnBase(char pos ){
  if(pos == 'r'){
	  Serial.println("Base Right");
      _base.write(CW_BASE);
  }else if(pos == 'c'){
	  Serial.println("Base center");
      _base.write(CENTER_BASE);
  }else if(pos == 'l'){
	  Serial.println("Base left");
      _base.write(CCW_BASE);
  }
}

void CameraArmDriver::turnShoulder(char pos ){
  if(pos == 'r'){
	  Serial.println("shoulder Right");
      _shoulder.write(CW_SHOULDER);
  }else if(pos == 'c'){
	  Serial.println("shoulder center");
      _shoulder.write(CENTER_SHOULDER);
  }else if(pos == 'l'){
      _shoulder.write(CCW_SHOULDER);
	  Serial.println("shoulder left");
  }
}
void CameraArmDriver::initPos(){
	turnBase('c');
	turnShoulder('r');
}

void CameraArmDriver::commandMode(){
	
	// print instruction 
	  Serial.println("\n\nServoTest commands:");

	  Serial.println("c : center servo");
	  Serial.println("l : servo left");
	  Serial.println("r : servo right");
	  Serial.println("<> 1 : base Servo ");
	  Serial.println("<> 2 : shoulder servo");
	  Serial.println("Usage: c 2 - move shoulder servo(2) to center.");
	  Serial.println("Usage: r 1 - move base servo(1) to the right.");

	 char ch;
    int s = 0;
	while(1){
		
		while (Serial.available() > 0) {
		ch = Serial.read();

		switch(ch) {

		case 'c':
		s = Serial.parseInt();
		Serial.print("ch: ");Serial.println(ch);
		Serial.print("s: "); Serial.println(s);
		if(s == 1){
		  Serial.println("Centering Base servo..");
		  turnBase('c');
		}else if(s == 2){
		  Serial.println("Centering Shoulder servo..");
		  turnShoulder('c');
		}
		break;
		case 'l':
		s = Serial.parseInt();
		if(s == 1){
		  Serial.println("Base Servo left..");
		  turnBase('l');
		}else if(s == 2){
		  Serial.println("Shoulder Servo left..");
		  turnShoulder('l');
		}
		break;
		case 'r':
		s = Serial.parseInt();
		if(s == 1){
		  Serial.println("Base Servo right..");
		  turnBase('r');
		}else if(s == 2){
		  Serial.println("Shoulder Servo right..");
		  turnShoulder('r');
		}
		break;
		}
	  }
	}
}
