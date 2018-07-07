#include <SabertoothSimplified.h>
SabertoothSimplified motordriver(Serial3);
void setup() {
  // put your setup code here, to run once:
  Serial3.begin(19200);
}

void loop() {
  // put your main code here, to run repeatedly:
    motordriver.motor(1, 127);
    //motordriver.motor(2, 127);
    while(1);
}
