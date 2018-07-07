
#include <NewPing.h>

#define SONAR_NUM     2 // Number or sensors.
#define MAX_DISTANCE 200 // Maximum distance (in cm) to ping.
#define PING_INTERVAL 33 // Milliseconds between sensor pings (29ms is about the min to avoid cross-sensor echo).

unsigned long pingTimer[SONAR_NUM]; // Holds the times when the next ping should happen for each sensor.
int cm[SONAR_NUM];         // Where the ping distances are stored.
uint8_t currentSensor = 0;          // Keeps track of which sensor is active.

NewPing sonar[SONAR_NUM] = {     // Sensor object array.
  NewPing(8, 9, MAX_DISTANCE), // Each sensor's trigger pin, echo pin, and max distance to ping.
  NewPing(10, 11, MAX_DISTANCE)
};
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

}

void loop() {
  // put your main code here, to run repeatedly:

  Serial.print("diff:"); Serial.println(sonarDistComparator());
  delay(1000);
  
}
void echoCheck() { // If ping received, set the sensor distance to array.
  if (sonar[currentSensor].check_timer())
    cm[currentSensor] = sonar[currentSensor].ping_result / US_ROUNDTRIP_CM;
}
void oneSensorCycle() { // Sensor ping cycle complete, do something with the results.
  Serial.println("========DEBUG=========");
  for (uint8_t i = 0; i < SONAR_NUM; i++) {
    Serial.print(i);
    Serial.print("=");
    Serial.print(cm[i]);
    Serial.print("cm ");
  }
  Serial.print("\tdiff:");
  Serial.print(cm[0]-cm[1]);
  Serial.print("==========================");
  Serial.println();
}
int sonarDistComparator(){
 int  leftdistance = sonar[0].ping_cm();
  int rightdistance = sonar[1].ping_cm();
  Serial.println("------------------");
  Serial.print("left:");Serial.print(leftdistance);
  Serial.print("\tright:");Serial.print(rightdistance);
  Serial.println("");
    return rightdistance-leftdistance; // with this year robot configuration #1 is the right. #0 is the left sonar 
    //if return (-)# left > right 
    // if retun (+)# right> left   
}


