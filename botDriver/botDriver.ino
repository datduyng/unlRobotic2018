
// these include is difference in all Operating System.
#include <SCC_Driver.h>


//2;r1;n,g1,n;


void setup() {
  Serial1.begin(115200);
  Serial.begin(115200);
  toFetal();

  toReady();
}
void loop() {
//          facePoint(COOR_X_1,COOR_Y_1,COOR_Z_1);
//          toPoint(COOR_X_1,COOR_Y_1,COOR_Z_1);
//          openClaw();
//          closeClaw();
//          facePoint(COOR_X_1,COOR_Y_1,COOR_Z_1);
//          toCane(COOR_X_DROP,COOR_Y_DROP,COOR_Z_DROP);
//          openClaw();
//          closeClaw();

  commandMode();
  while(1);
  
//  getDataStream();
//  if(parseData()){
//    printSimplePoint();
//   
//
//    while(1);
//  }
}
