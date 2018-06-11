#include<Driving.h>

void setup() {
 // SabertoothSimplified motordriver(Serial3); 
  // put your setup code here, to run once:
  dinit();
  debugMode();
//      motordriver.motor(1, 100);
//    motordriver.motor(2, 200);
}

void loop() {
  // put your main code here, to run repeatedly:
  driveto(12);
  debugMode();
  delay(100);
}
