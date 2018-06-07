
// these include is difference in all Operating System.
//#include "lib\SCC_Driver\SCC_Driver.h"
#include "lib\ballCoordinate\ballCoordinate.h"
#include<CameraArmDriver.h>

CameraArmDriver cameraArm;


void setup() {
  Serial.begin(19200);
  cameraArm.begin(10,11);
}

void loop() {
  
}

void testCameraArm(){
    Serial.println("Hello");
  int baseAngle = cameraArm.getCurrentBaseAngle();
  int shoulderAngle = cameraArm.getCurrentShoulderAngle();
  Serial.print("base:");Serial.print(baseAngle);Serial.print("  Shoulder:");
  Serial.println(shoulderAngle);
  //cameraArm.turnShoulder(20);
cameraArm.turnBase(45);
  //cameraArm.turnBase(180);

  //cameraArm.sweepBase(2);
  while(1){};
}

