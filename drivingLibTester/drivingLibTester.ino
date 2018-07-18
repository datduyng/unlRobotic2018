#include<Driving.h>
/**
 * BUG: some tim ecannot use the same function again the  bot jsut stio 
 */
void setup() {
 // SabertoothSimplified motordriver(Serial3); 
  // put your setup code here, to run once:
  dinit();
//  debugMode();                                              
  //imuInit();
 // Serial3.begin(19200);
//      motordriver.motor(1, 100);
//    motordriver.motor(2, 200);
}

void loop() {
  // put your main code here, to run repeatedly:
  driveto(10);
  delay(100);
    driveto(10);
  delay(100);
    driveto(10);
  delay(100);
    driveto(10);
  delay(100);
//  steer(90);
//   delay(1000);
//  steer(-90);
//     delay(1000);
//
//       driveto(10);
////  delay(100);
//  steer(90);
//   delay(1000);
//  steer(-90);
//     delay(1000);
//
//       driveto(10);
////  delay(100);
//  steer(90);
//   delay(1000);
//  steer(-90);
//     delay(1000);
//
//       driveto(10);
////  delay(100);
//  steer(90);
//   delay(1000);
//  steer(-90);
//     delay(1000);
//driveto(20);
//delay(100);
//steer(90);
//delay(100);
//driveto(10);
  /* competition drive path
  driveto(71.25);
  delay(100);
  steer(90);
  delay(100);
  driveto(22.3125);
  delay(1000);
  steer(90);
  delay(1000);
  driveto(71.25);
  */

//driveto(20);
  while(1);
  // 71.
  // 22.3125 in 
}
