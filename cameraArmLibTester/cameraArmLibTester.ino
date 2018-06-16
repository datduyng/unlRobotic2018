#include<CameraArmDriver.h>

CameraArmDriver cameraArm;
void setup() {
  // put your setup code here, to run once:
    Serial.begin(115200);
    cameraArm.begin(6,9);
    cameraArm.commandMode();

}

void loop() {

   
}// end loopp 


