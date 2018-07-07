  // 71.25 // competition
  // 22.3125 in 

#define NO_ROW 4 
void setup() {
  dinit();// the sonar will init also . 

}

void loop() {
  // put your main code here, to run repeatedly:
  for(int i = 0; i < NO_ROW;i++){
    //TODO: 
    driveto(70);// actual: 71.25
    // competition drive sequence go here . 
    
    ///////////////////on't turn at the end of last row. 
    
    if(distComp >= 1 && i != NO_ROW-1){// turn right if right is open 
      steer(90);
      delay(100
    int distComp = sonarDistComparator();
    // don't turn at the end of last row. 
    if(distComp >= 1 && i != NO_ROW-1){// turn right if right is open 
      steer(90);
      delay(100);
      driveto(21);// actual: 22.3125
      steer(90);
    }else if(distComp <= 1&& i != NO_ROW-1){// turn left if left is open 
      steer(-90);
      delay(100);
      driveto(21);// actual: 22.3125
      steer(-90)
    }else{
      // equal.
      // some malfunction maybe ???
      
    }
  }
  driveto(10);
}
