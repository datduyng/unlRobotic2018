  // 71.25 // competition
  // 22.3125 in 

  //18in first move in row 
  //10 inches from there on. 
#include<Driving.h>
#include <CameraArmDriver.h>

CameraArmDriver cameraArm;
const int NO_ROW= 4 ;
#define NO_STOP_PER_ROW 5 // the last stop is half of 1 regular stop. 
void setup() {
  dinit();// the sonar will init also . 
//  Serial.begin(9600);
  cameraArm.cameraArmBegin(6, 7);
  cameraArm.rest();
}

void loop() {
  // put your main code here, to run repeatedly:
  for(int i = 0; i < NO_ROW;i++){
    //TODO: 
    delay(100);

    //run per row 
    for(int noStop = 0;noStop < NO_STOP_PER_ROW;noStop++){
      if(noStop == 0){// first stop 
        driveto(18);
      }else if(noStop == NO_STOP_PER_ROW-1){// last stop 
        driveto(5);
      }else{
        driveto(10);
      }

      if(getDataStream()){
        Serial.println("got package");
      }
      // trigger the camera 
      parseData();

      //TODO: 
      
      //raise camera, take picture, then process image. 
      cameraArm.turn('r');
      delay(2000);
      cameraArm.turn('l');
      delay(1000);
      cameraArm.rest();
      delay(1000);
      //TODO: process each ball here 
      
    }
    
    driveto(19.5);
    // total move: 71.25 inches
    // competition drive sequence go here . 

    int distComp = sonarDistComparator();
    

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
