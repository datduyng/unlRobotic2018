
// these include is difference in all Operating System.
#include "C:\Users\datng\OneDrive\pPlayground\arduino\project\unlRobotic2018\botDriver\lib\SCC_Driver\SCC_Driver.h"
#include "C:\Users\datng\OneDrive\pPlayground\arduino\project\unlRobotic2018\botDriver\lib\ballCoordinate\ballCoordinate.h"
#include "C:\Users\datng\OneDrive\pPlayground\arduino\project\unlRobotic2018\botDriver\lib\CameraArmDriver\CameraArmDriver.h"

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
//    for(int i =0; i<4; i++){
//      if(color[i]>0){
//        switch(i){
//          case 0:
//          //
//          toCane(COOR_X_1,COOR_Y_1,COOR_Z_1);
//          openClaw();
//          closeClaw();
//          toCane(COOR_X_DROP,COOR_Y_DROP,COOR_Z_DROP);
//          openClaw();
//          closeClaw();
//          break;
//
//          case 1:
//          //facePoint(COOR_X_2,COOR_Y_2,COOR_Z_2);
//          toCane(COOR_X_2,COOR_Y_2,COOR_Z_2);
//          openClaw();
//          closeClaw();
//          toCane(COOR_X_DROP,COOR_Y_DROP,COOR_Z_DROP);
//          break;
//
//          case 2:
//          //facePoint(COOR_X_3,COOR_Y_3,COOR_Z_3);
//          toCane(COOR_X_3,COOR_Y_3,COOR_Z_3);
//          openClaw();
//          closeClaw();
//          toCane(COOR_X_DROP,COOR_Y_DROP,COOR_Z_DROP);
//          break;
//
//          case 3:
//          //facePoint(COOR_X_4,COOR_Y_4,COOR_Z_4);
//          toCane(COOR_X_4,COOR_Y_4,COOR_Z_4);
//          openClaw();
//          closeClaw();
//          toCane(COOR_X_DROP,COOR_Y_DROP,COOR_Z_DROP);
//          break;
//        }
//      }
//    }// end for
//
//    while(1);
//  }
}
