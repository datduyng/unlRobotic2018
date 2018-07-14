#include<Driving.h>
void setup() {
  // put your setup code here, to run once:
  dinit();
}
const int left_offset = -1; 
const int right_offset = 1; 

void loop() {
  // put your main code here, to run repeatedly:
    Serial.print(getSonarLeftDistance() + left_offset );
    Serial.print(",");
    Serial.print(getSonarRightDistance()+right_offset);
    Serial.println();
    delay(1000);
}
