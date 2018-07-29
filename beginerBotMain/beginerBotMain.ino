/**
 * @Author Mechatronic(MECH857) Team + Robert Goldsworthy(modified by jul 26th, 2018)
 * modified by: Dat Nguyen(jul 28, 2018)  
 * 
 */
//https://www.robotshop.com/en/parallax-aluminum-motor-mount-wheel-kit.html
//144 encoder positions per revolution (0.14-inch of linear travel accuracy)
//https://www.dimensionengineering.com/software/SabertoothArduinoLibrary/html/class_sabertooth.html

//#include <SCC_Driver.h>
#include <NewPing.h>
#include <Sabertooth.h>

//Set up Ultrasonic Sensors:
#define SONAR_NUM 2      // Number of sensors.
#define MAX_DISTANCE 200 // Maximum distance (in cm) to ping.
NewPing sonar[SONAR_NUM] = {   // Sensor object array.
NewPing(30, 31, MAX_DISTANCE), // Each sensor's trigger pin, echo pin, and max distance to ping.
NewPing(52, 53, MAX_DISTANCE), 
};

Sabertooth ST(128);

/*global variable */
char startChar = -1;
byte incomingByte;
int i = 1;
int m = 0;
float x = 8.7;
float y;
float z;
boolean y_label;
boolean z_label;
boolean go = LOW;
boolean arm = LOW;
int totalrows = 5;
int pntperRow = 5;
int pntCount;
bool fwd = LOW;
double leftdist;
double rightdist;
double turnval=0;
int j = 0;
int distCNT = 58 ;
double turnscale =  .6;
bool turnbool = LOW;
int turnCNT = 143;
double drivecmd = -20;


void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);
  //toReady();
  //closeClaw(); 
  Serial2.begin(9600);
  
  ST.autobaud();
  ST.drive(0);
  ST.turn(0);
  
}

void loop() {
  // put your main code here, to run repeatedly:

}



Sabertooth ST(128);

long CntCreator(int in) {
  long power = in*1000/24;
  return power;
}

void forward(int lngth) {
    int cnt;
  long Cnt = CntCreator(lngth); //enter # of inches desired in function encoderCntCreator
  while(cnt <= Cnt){
// Move robot a normal length
leftdist = sonar[0].ping_cm();
rightdist = sonar[1].ping_cm();
turnval = 10*(leftdist - rightdist);
turnval = constrain(turnval,-2,2);;
ST.turn(turnval);
ST.drive(-20);
//ST.motor(1,-20);
//ST.motor(2,-22.75);
cnt++;
}
ST.drive(0);
ST.turn(0);
}

void turnleft(){
  ST.turn(6);
  ST.drive(-20);
  delay(15000);
  ST.turn(0);
  ST.drive(0);
}
void turnright(){
  ST.turn(-6);
  ST.drive(-20);
  delay(15000);
  ST.turn(0);
  ST.drive(0);
}
