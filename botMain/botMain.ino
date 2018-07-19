  // 71.25 // competition
  /**
   * '1':   ;0,7.5,7.5,11;
   * '2':   ;0,7.5,2.25,11;
   * '3':   ;0,7.5,7.5,7;
   * '4':   ;0,7.5,2.25,7;
   * ;0,7.5,7.5,11;0,7.5,2.25,11;0,7.5,7.5,7;0,7.5,2.25,7;
   */

//
/**
 * R: 0
 * B: 1
 * G : 2
 */
  //18in first move in row
  //10 inches from there on.
#include<Driving.h>
#include <CameraArmDriver.h>
#include <SCC_Driver.h>
#include <ballCoordinate.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// initialize the interface pins
LiquidCrystal_I2C lcd(0x27, 16, 2); // set address 
CameraArmDriver cameraArm;

const int NO_ROW= 4 ;
#define NO_STOP_PER_ROW 5 // the last stop is half of 1 regular stop.
#define led 22

bool firstStart = true;

void initRobotArmPos();

void setup() {
 Serial.begin(115200);


 // sets the LCD's rows and colums:
 lcd.begin();
 lcd.backlight(); //backlight is now ON


  dinit();// the sonar will init also .
//  Serial.begin(9600);
  cameraArm.cameraArmBegin(6, 7);
  cameraArm.rest();
  pinMode(led,OUTPUT);
  digitalWrite(led,LOW);
  delay(5000);
  initRobotArmPos();
  firstStart = true;

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
      if(noStop == 0 && firstStart != true){// first stop
        Serial.println("first stop of each row but 1st row drive 12");
//        driveto(12);
      }else if(noStop == NO_STOP_PER_ROW-1&& firstStart != true){// last stop
//        if(abs(wallOffset) >= 3) goParallel(5,leftDist,rightDist); // the wall, if not fix it self.
//        else                     driveto(5);
        Serial.println("drive 5(last)"); 
      }else if(firstStart != true){
//        if(abs(wallOffset) >= 3) goParallel(10,leftDist,rightDist);
//        else                     driveto(10);
        Serial.println("drive 10(normal)"); 
      }
      firstStart = false;
      //check parallel everytime the bot stop 
//      checkParallel(); 

      // trigger the camera


      //TODO:
      toReady(); // arm to ready position
      //raise camera, take picture, then process image.
      cameraArm.turn('r');
      Serial.print("arm pos");Serial.println(cameraArm.getCameraFacePosition());

      if(getDataStream(cameraArm.getCameraFacePosition())){
        Serial.println("got package");
      }else{
        digitalWrite(led,HIGH);
        delay(1000);
        digitalWrite(led,LOW);
        delay(1000);
      }
      delay(1000);
      cameraArm.rest()  ;
      delay(1000);
      processRobotArm(parseData());

      cameraArm.turn('l');

      if(getDataStream(cameraArm.getCameraFacePosition())){
        Serial.println("got package");
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
       processRobotArm(parseData());

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

void processRobotArm(bool isBall){
  Serial.print("numpoint: ");Serial.println(numOfPoint);

  if(isBall == true){// process arm if there is ball
    for(int i =0; i<numOfPoint; i++){
      if(color[i] == 0){ // red
          toPoint(x[i],y[i],z[i]);
          openClaw();
          closeClaw();

          //TODO: change to deposit
          depositItem();
//          toPoint(x[i],y[i],z[i]);
//          openClaw();
//          closeClaw();
      }else if(color[i] == 2){// blue
          toPoint(x[i],y[i],z[i]);
          openClaw();
          closeClaw();

          //TODO: change to deposit
//          toPoint(x[i],y[i],z[i]);
//          openClaw();
//          closeClaw();
      }else{

      }

      //GO back to ready position after every time the arm pick apple.
      toReady();
    }// end for
  }else{ // don't process if there is no ball.

  }

}
