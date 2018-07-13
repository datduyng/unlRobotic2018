  // 71.25 // competition
  // 22.3125 in 

  //18in first move in row 
  //10 inches from there on. 
#include<Driving.h>
#include <CameraArmDriver.h>
void setup() {
  // put your setup code here, to run once:
  dinit();// the sonar will init also 
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  int stop = 12; 
  Serial.println("==================================");
  for(int i = 0; i < stop; i++){
    Serial.print(getSonarLeftDistance());
    Serial.print(",");
    Serial.print(getSonarRightDistance());
    Serial.println();
    delay(1000);
    //TODO; print dist 

    driveto(2);
    
  }
  Serial.println("=================================");
  while(1);
}
