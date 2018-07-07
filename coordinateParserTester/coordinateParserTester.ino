//2;R,23,12;G,12,13";
// 1;23,12,42
//2;66,34,12;123,124,12
//2;r1;n,g1,n;
//2;0;66;0;123;

#include<ballCoordinate.h>

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(2,OUTPUT);
  pinMode(3,OUTPUT);
  digitalWrite(2,LOW);
  digitalWrite(3,LOW);
   

}

void loop() {
  // put your main code here, to run repeatedly:

     bool right = getDataStream();
//  if(parseData()){
//    Serial.println("Success parse");
//    digitalWrite(2,HIGH);
//    delay(1000);
//    digitalWrite(2,LOW);
//    delay(1000);
//    while(1);
//  }else{
//    Serial.println("fail parse");
//    digitalWrite(3,HIGH);
//    delay(1000);
//    digitalWrite(3,LOW);
//    delay(1000);
//    while(1);
//  }
//    digitalWrite(13,HIGH);
//    delay(1000);
//    digitalWrite(13,LOW);
//    delay(1000);
//
//    delay(1000);
if(parseSimpleData()){
  Serial.println("Success fully parse");
  printSimplePoint();
}else{
  Serial.println("fail Parsing");
}
printSimplePoint();
}

