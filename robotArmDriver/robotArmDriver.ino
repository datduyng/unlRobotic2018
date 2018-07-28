//ard
// these include is difference in all Operating System.

#define botConst1
#include<botConst.h>
#include <SCC_Driver.h>


void setup() {
  Serial1.begin(115200);
  Serial.begin(115200);
  
  toFetal();

  toReady();
}
void loop() {


  commandMode();
  while(1);

}
