  // 71.25 // competition
  // 22.3125 in 

  //18in first move in row 
  //10 inches from there on. 
#include<Driving.h>
#include <CameraArmDriver.h>
#include <SCC_Driver.h>
#include <ballCoordinate.h>

CameraArmDriver cameraArm;
const int NO_ROW= 4 ;
#define NO_STOP_PER_ROW 5 // the last stop is half of 1 regular stop. 
#define led 22 

void initRobotArmPos();

void setup() {
 Serial.begin(115200);
  dinit();// the sonar will init also . 
//  Serial.begin(9600);
  cameraArm.cameraArmBegin(6, 7);
  cameraArm.rest();
  pinMode(led,OUTPUT);
  digitalWrite(led,LOW);
  delay(5000);
  initRobotArmPos();

}

void loop() {
  // put your main code here, to run repeatedly:
  for(int i = 0; i < NO_ROW;i++){
    //TODO: 
    delay(100);

    //run per row 
    for(int noStop = 0;noStop < NO_STOP_PER_ROW;noStop++){
      
        int rightDist =  getSonarRightDistance();
        int leftDist =  getSonarLeftDistance();
        int wallOffset = rightDist - leftDist; // make sure that the bot is parallel to
      if(noStop == 0){// first stop 
        driveto(12);
      }else if(noStop == NO_STOP_PER_ROW-1){// last stop 
        if(abs(wallOffset) >= 3) goParallel(5,leftDist,rightDist); // the wall, if not fix it self. 
        else                     driveto(5);
        
      }else{
        if(abs(wallOffset) >= 3) goParallel(10,leftDist,rightDist);
        else                     driveto(10);
      }


      // trigger the camera 
     

      //TODO: 
      toReady(); // arm to ready position 
      //raise camera, take picture, then process image. 
      cameraArm.turn('r');
      Serial.print("arm pos");Serial.println(cameraArm.getCameraFacePosition());
      
      if(getDataStream(cameraArm.getCameraFacePosition())){
        Serial.println("got package");
        parseData();
      }else{
        digitalWrite(led,HIGH);
        delay(1000);
        digitalWrite(led,LOW);
        delay(1000);
      }
      delay(1000);
      cameraArm.rest()  ;
      delay(1000);
      processRobotArm();
        
      cameraArm.turn('l');
      
      if(getDataStream(cameraArm.getCameraFacePosition())){
        Serial.println("got package");
        parseData();
      }else{
        digitalWrite(led,HIGH);
        delay(1000);
        digitalWrite(led,LOW);
        delay(1000);
      }
      delay(1000);
      cameraArm.rest()  ;
      delay(1000);
      // go pci each ball. 
       processRobotArm();

    }
    
    driveto(19.5);
    // total move: 71.25 inches
    // competition drive sequence go here . 
    
    int distComp = sonarDistComparator();

    Serial.println("=====================");
    Serial.println("distComp");Serial.print(distComp);
    Serial.println();

    // if have one robot running 
    if(i==NO_ROW-2){// reverse the sign at the last turn
     distComp = -distComp;
    }

    // don't turn at the end of last row. 
    if(distComp >= 1 && i != NO_ROW-1){// turn right if right is open 
      delay(100);
      steer(90);
      delay(100);
      driveto(21);// actual: 22.3125
      delay(100);
      steer(90);
    }else if(distComp <= 1&& i != NO_ROW-1){// turn left if left is open 
      delay(100);
      steer(-90);
      delay(100);
      driveto(21);// actual: 22.3125
      delay(100);
      steer(-90);
    }else{
      // equal.
      // some malfunction maybe ???
    }
  }// end for 
  while(1);
}// end loop 

void initRobotArmPos(){
    toFetal();
  toReady();
}

void processRobotArm(){
   for(int i =0; i<4; i++){
      if(color[i]>0){
        switch(i){
          case 0:
          //
          toCane(COOR_X_1,COOR_Y_1,COOR_Z_1);
          openClaw();
          closeClaw();
          toCane(COOR_X_DROP,COOR_Y_DROP,COOR_Z_DROP);
          openClaw();
          closeClaw();
          break;

          case 1:
          //facePoint(COOR_X_2,COOR_Y_2,COOR_Z_2);
          toCane(COOR_X_2,COOR_Y_2,COOR_Z_2);
          openClaw();
          closeClaw();
          toCane(COOR_X_DROP,COOR_Y_DROP,COOR_Z_DROP);
          break;

          case 2:
          //facePoint(COOR_X_3,COOR_Y_3,COOR_Z_3);
          toCane(COOR_X_3,COOR_Y_3,COOR_Z_3);
          openClaw();
          closeClaw();
          toCane(COOR_X_DROP,COOR_Y_DROP,COOR_Z_DROP);
          break;

          case 3:
          //facePoint(COOR_X_4,COOR_Y_4,COOR_Z_4);
          toCane(COOR_X_4,COOR_Y_4,COOR_Z_4);
          openClaw();
          closeClaw();
          toCane(COOR_X_DROP,COOR_Y_DROP,COOR_Z_DROP);
          break;
        }
      }
    }// end for
}

