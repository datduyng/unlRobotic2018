
#include<ballCoordinate.h>

char *dataString = "2;R,23,12;G,12,13";
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  getDataStream();
  parseData();
//  char *str = toString();
  Serial.print("NUMOFPOINT:IDE");Serial.println(numOfPoint);
  Serial.print("Ball.y.out");Serial.println(balls[0].y);
  Serial.print("Ball.z.out");Serial.println(balls[0].z);
  if(balls[0].color == 'R'){
    Serial.println("is equal");
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}
