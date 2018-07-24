#include<Driving.h> 
int sonarOffset[4] = {-3,-2, -3,0};

void setup() {
  // put your setup code here, to run once:
  dinit(); 
  Serial.begin(115200); 
    setSonarOffset(sonarOffset);//update offset 7/23/18
}

  void loop() {
    // put your main code here, to run repeatedly:
    Serial.print("left: ");Serial.print(getSonarLeft());
   Serial.print(", right: ");Serial.print(getSonarRight());
  Serial.print(", back: ");Serial.print(getSonarLeftBack());
Serial.println();
  delay(1000);
}
