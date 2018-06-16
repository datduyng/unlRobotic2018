
// these include is difference in all Operating System.
#include "lib\SCC_Driver\SCC_Driver.h"
#include "lib\ballCoordinate\ballCoordinate.h"
#include<CameraArmDriver.h>

CameraArmDriver cameraArm;


void setup() {
  Serial.begin(115200);
  cameraArm.begin(10,11);
}

void loop() {
  
}
