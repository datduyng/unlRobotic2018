#include<Driving.h> 
int sonarOffset[5] = {-1,-1, -1,-1,-1};

void setup() {
  // put your setup code here, to run once:
  dinit(); 
  Serial.begin(115200); 
    setSonarOffset(sonarOffset);//update offset 7/23/18
//   initSonarTimer();
}

  void loop() {
    // put your main code here, to run repeatedly:
//    updateSonar();
    Serial.print("left: ");Serial.print(getSonarLeft());
   Serial.print(", right: ");Serial.print(getSonarRight());
  Serial.print(", backLeft: ");Serial.print(getSonarLeftBack());
  Serial.print(", Front: ");Serial.print(getSonarFront());
  Serial.print(", Back: ");Serial.print(getSonarBack());
  Serial.println();

  delay(1000);
}
