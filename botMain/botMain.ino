  // 71.25 // competition
  /**
   * '1':   ;0,7.5,7.5,11;
   * '2':   ;0,7.5,2.25,11;
   * '3':   ;0,7.5,7.5,7;
   * '4':   ;0,7.5,2.25,7;
   * ;0,7.5,7.5,11;0,7.5,2.25,11;0,7.5,7.5,7;0,7.5,2.25,7;
   */
  //TODO: add OFFSET from camera arm to robotarm is .158 inch 
//
/**
 * R: 0
 * B: 1
 * G : 2
 * This program test the sequence drive of the bot.
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
const int sonarOffset[SONAR_NUM] = {-1,-1, -1,-2,-1};

bool firstStart = true;

/**
 * This function check if the bot is at the right position to turn.
 * return distance that the bot  that it has to travel to fix. 
 */
float getReadyToTurn();

/**
 * This function give live reading from 4 sonar to lcd. 
 * for debuging. adjusting robot at the beginning of the competition. 
 * param: period ( that you want this process to go) 
 * param: refresh ( refresh rate of the lcd screen)
 * return none 
 */
void checkSonar(long period, int refresh);

/*
 * This function init the robot arm position
 */
void initRobotArmPos();

/*
 * Process the robot arm once retrieve data from pi 
 */
void processRobotArm(bool isBall);

/**
 * Turn Left sequence at the end of the row
 */
void turnLeftSequence();

/**
 * Turn right sequence at the end of the row. 
 */
void turnRightSequence();

void setup() {
 Serial.begin(115200);


 // sets the LCD's rows and colums:
 lcd.begin();
 lcd.backlight(); //backlight is now ON


  dinit();// the sonar will init also .

  //set sonar offset. 
  setSonarOffset(sonarOffset);//update offset 7/23/18
//  Serial.begin(9600);
  cameraArm.cameraArmBegin(6, 7);
  cameraArm.rest();
  
  initRobotArmPos();
  firstStart = true;

  lcd.clear();
  lcd.setCursor(2,0);
  lcd.print("Hello Friend! ");
  lcd.setCursor(0,1);
  lcd.print("I'm countin on u.....");
   checkSonar(5000,100);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  for(int i = 0; i < NO_ROW;i++){
    //TODO:
    delay(100);

    processPerRow();

    lcd.clear();
    lcd.print(String("end of row:")+String(i));
    driveto(last_frame_to_wall);
    // total move: 71.25 inches
    // competition drive sequence go here .

    int distComp = sonarDistComparator();

    lcd.clear();
    lcd.print("open row?-O"+String(distComp));

    // if have one robot running
    if(i==NO_ROW-2){// reverse the sign at the last turn
     distComp = -distComp;
    }

    // don't turn at the end of last row.
    if(distComp >= 1 && i != NO_ROW-1){// turn right if right is open
      turnRightSequence();
    }else if(distComp <= 1&& i != NO_ROW-1){// turn left if left is open
      turnLeftSequence(); 
    }else{
      // equal.
      // some malfunction maybe ???
    }
  }// end for(NO_ROW)
  while(1);
}// end loop


void turnLeftSequence(){
        lcd.clear();
      lcd.print("turn left....");
      delay(100);

      //fix the bot distance make sure it has reach the right point to turn 
      getReadyToTurn();
      
      delay(100);
      steer(-90);
      delay(100);
      driveto(dist_between_2_row);// actual: 22.3125
      //turn begin at the new row. 
      delay(100);
      steer(-90);
}

void turnRightSequence(){
        lcd.clear();
      lcd.print("turn right....");
      delay(100);
      
      //fix the bot distance make sure it has reach the right point to turn 
      getReadyToTurn();

      delay(100);
      steer(90);
      delay(100);
      driveto(dist_between_2_row);// actual: 22.3125
      //turn begin at the new row. 
      delay(100);
      steer(90);
}
void initRobotArmPos(){
    lcd.clear();
    lcd.print("init arm");
    toFetal();
    toReady();
}

void processRobotArm(bool isBall){
//  Serial.print("numpoint: ");Serial.println(numOfPoint);

  if(isBall == true){// process arm if there is ball
    lcd.clear();
    lcd.print(String("Processing--")+
              String(numOfPoint)+
              String("b"));
              
    for(int i =0; i<numOfPoint; i++){
      if(color[i] == 0){ // red
          lcd.setCursor(0,1);
          lcd.print(String(color[i])+String(",")+
          String(x[i])+String(",")+
          String(y[i])+String(",")+
          String(z[i]));

          facePoint(x[i],y[i],z[i]);
          toCane(x[i],y[i],z[i]);
          openClaw();
          closeClaw();

          //TODO: change to deposit
          depositItem();
//          toPoint(x[i],y[i],z[i]);
//          openClaw();
//          closeClaw();
      }else if(color[i] == 2){// blue
          lcd.setCursor(0,1);
          lcd.print(String(color[i])+String(",")+
          String(x[i])+String(",")+
          String(y[i])+String(",")+
          String(z[i]));
          
          facePoint(x[i],y[i],z[i]);
          toCane(x[i],y[i],z[i]);
          openClaw();
          closeClaw();

          //TODO: change to deposit
//          toPoint(x[i],y[i],z[i]);
//          openClaw();
//          closeClaw();
      }else{
           lcd.clear();
           lcd.print("color???????");
      }

      //GO back to ready position after every time the arm pick apple.
      toReady();
    }// end for
  }else{ // don't process if there is no ball.
       lcd.clear();
       lcd.print("no apple at this zone. ");
  }

}

void processPerRow(){
      //run per row
    for(int noStop = 0;noStop < NO_STOP_PER_ROW;noStop++){

        //make assumption that first stop of the row is perfect. 
        
        int rightDist =  getSonarRight();
        int leftDist =  getSonarLeft();
        int wallOffset = rightDist - leftDist; // make sure that the bot is parallel to
      if(noStop == 0 && firstStart == false){// first stop
          lcd.clear();
          lcd.print("begin of 1 row");
          driveto(dist_to_first_col);
      }else if(noStop == NO_STOP_PER_ROW-1&& firstStart == false){// last stop
        if(abs(wallOffset) >= wallOffsetThresh){
          lcd.clear();
          lcd.print("NP-last-go 5");
          lcd.setCursor(0,1);
          lcd.print("fixing....");
          goParallel(last_dist_2_frame,leftDist,rightDist); // the wall, if not fix it self.
        }
        else{
          lcd.clear();
          lcd.print("Par-last-go 5");
          //driveto(5);
          drivetoSonarFeedback(last_dist_2_frame,'f',true);
        }
        
      }else if(firstStart == false){ // normal stop 
        if(abs(wallOffset) >= wallOffsetThresh){
          lcd.clear();
          lcd.print("NP-mid-go 10");
          lcd.setCursor(0,1);
          lcd.print("fixing....");
          goParallel(dist_2_frame,leftDist,rightDist);
        }
        else{
          lcd.clear();
          lcd.print("Par-mid-go 10");
//          driveto(10);
          drivetoSonarFeedback(dist_2_frame,'f',true);
        }

          //check parallel everytime the bot stop 
          lcd.clear();
          lcd.print("Check Parall..");
          int offBy = checkParallel(); 
    
          lcd.setCursor(0,1);
          lcd.print(String("turn: ")+
                String(offBy));
                
      }
      firstStart = false;
      
      // trigger the camera
      //TODO:
      lcd.clear();
      lcd.print("Arm toReady");
      toReady(); // arm to ready position


      
      //raise camera, take picture, then process image.
      cameraArm.turn('r');
      lcd.clear();
      lcd.print(String("arm pos: ")+
                String(cameraArm.getCameraFacePosition()) );

      // update before sending to rasp.
      rightDist =  getSonarRight();
      leftDist =  getSonarLeft();
      wallOffset = (rightDist - leftDist)/2; // make sure that the bot is parallel to
      char packageToPi[30];

      //don't trust reading if sonaroffset is 1111 or -1111 
      sprintf(packageToPi,";%d,%d;",wallOffset,cameraArm.getCameraFacePosition());

      lcd.clear();
      lcd.print(String("Package send to rasp"));
      lcd.setCursor(2,1);
      lcd.print(String(packageToPi));

      if(getDataStream(packageToPi)){
        lcd.clear();
        lcd.print("got package");
      }else{
        lcd.clear();
        lcd.print("NA package ");
      }
      delay(1000);
      cameraArm.rest()  ;
      delay(1000);
      bool gotPackage = parseData();
      if(gotPackage){
        lcd.clear();
        lcd.print("parsing Package");
      }else{
        lcd.clear();
        lcd.print("no Package");
      }
      
      lcd.setCursor(0,1);
      lcd.print("processing arm");
      processRobotArm(gotPackage);

      cameraArm.turn('l');
      
      lcd.clear();
      lcd.print(String("arm pos: ")+
                String(cameraArm.getCameraFacePosition()) );

      //don't trust reading if sonaroffset is 1111 or -1111 
      sprintf(packageToPi,";%d,%d;",wallOffset,cameraArm.getCameraFacePosition());

      lcd.clear();
      lcd.print(String("Package send to rasp"));
      lcd.setCursor(2,1);
      lcd.print(String(packageToPi));
      
      if(getDataStream(packageToPi)){
        lcd.clear();
        lcd.print("got package");
      }else{
        lcd.clear();
        lcd.print("NA package ");
      }
      delay(1000);
      cameraArm.rest()  ;
      delay(1000);
      // go pci each ball.
       processRobotArm(parseData());

    }//end for NO STOP
}// end processPerRow()
/**
 * This function check if the bot is at the right position to turn.
 */
float getReadyToTurn(){
  //11 5/8 inch is position to turn the bot.
  //13 cm from the side.s

  //checkParallel() before use getFrontSonar dist 
//  checkParallel();
  int frontDist = getSonarFront();

  //don't trust the reading 
  // malfunction on the frontDist. 
  if(frontDist <= 0) return 404.0;
  
  float distFix = (float)(frontDist - beforeTurnDist) / CM_TO_INCH; 

  lcd.clear();
  lcd.print(String("F")+String(frontDist));
  lcd.setCursor(0,1);
  lcd.print(String("Fixed ")+
            String(distFix)+ String("in"));
            
  driveto(distFix);
  return distFix;
}

void checkSonar(long period, int refresh){
    long prev= millis();
    long timer = 0; 
   while( timer < period){
        timer += (millis() - prev);
        prev = millis();
        lcd.clear(); 
        lcd.print(String("L")+String(getSonarLeft())+
                  String(" R")+String(getSonarRight())+
                  String(" BL")+String(getSonarLeftBack())+
                  String(" F")+String(getSonarFront()));

        lcd.setCursor(0,1);
        lcd.print(String(" B")+ String(getSonarBack()));
        delay(refresh);
    }
  
}
