#include<CameraArmDriver.h>

CameraArmDriver cameraArm;
void setup() {
  // put your setup code here, to run once:
//    Serial.begin(115200);
    cameraArm.cameraArmBegin(6,7);
    cameraArm.commandMode();

}

void loop() {

   
}// end loopp 


