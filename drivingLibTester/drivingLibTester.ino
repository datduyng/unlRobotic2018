#include<Driving.h>
/**
 * BUG: some tim ecannot use the same function again the  bot jsut stio 
 */
void setup() {
 // SabertoothSimplified motordriver(Serial3); 
  // put your setup code here, to run once:
  dinit();
  Serial.begin(115200);
  setSonarOffset(-1,-2, 0);
}

void loop() {
  // put your main code here, to run repeatedly:
  checkParallel();
  delay(1000);

  delay(3000);
  checkParallel();
  while(1);
}
