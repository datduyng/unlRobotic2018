/**
 * This program test the offset offset of all sonar. 
 * det the offset of 3rd sonar(back sonar) 
 * to the left front sonar. 
 */
#include<Driving.h>
void setup() {
  // put your setup code here, to run once:
  dinit();
}
const int left_offset = -1; 
const int right_offset = 1; 

void loop() {
  // put your main code here, to run repeatedly:
    Serial.print(getSonarLeftDistance());
    Serial.print(",");
    Serial.print(getSonarRightDistance());
    Serial.print(",");
    Serial.print(getSonarBackDistance());
    Serial.println(",");
    delay(1000);
}
