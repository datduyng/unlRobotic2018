//2;R,23,12;G,12,13";
// 1;23,12,42
//2;66,34,12;123,124,12

#include<ballCoordinate.h>

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  getDataStream();
  if(parseData()){
    Serial.println("Success parse");
  }else{
    Serial.println("fail parse");
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}
