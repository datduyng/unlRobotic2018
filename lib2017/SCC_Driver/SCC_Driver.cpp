#include "SCC_Driver.h"
#include "Arduino.h"
#include "math.h"
#include "stdlib.h"
#include "stdint.h"

float currConfig[4];
float targetConfig[4];
float prevPos[3];
float currPos[3];
float targetPos[3];
float comparePos[3];
int duration;


// ARM POSITIONING AND CONFIGURATION ---------------------------------------------------------------------------//

void toConfiguration(float angles[], int duration){
  // Servo Motors have different standard control signals. Converting input angles to servo signals
  //  requires control singal range information for each servo in the Lynxmotion AL5D 4-DOF arm.
  //
  //  (Collected from datasheets)
  //  BASE SERVO:	     HS-475HB     900-2100 µs : 0-200 degrees
  //  SHOULDER SERVO:    HS-805BB     556-2420 μs : 0-199.5 degrees
  //  ELBOW SERVO:       HS-755HB     0-180 μs : 0-202 degrees
  //  WRIST SERVO:       HS-645MG     553-2520 μs : 0-197 degrees
  //  GRIPPER SERVO:     HS-422       1500 μs     : neutral
  //
  //  Emperically determined in operation (neglecting the use of the base-to-elbow spring)
  //  BASE SERVO:       [0-180, 580-2300µs]
  //  SHOULDER SERVO    [0-90,  750-2375µs]
  //  ELBOW SERVO       [0-90,  830-1700µs] 
  //  WRIST SERVO       [0-180, 660-2475µs]
  //  GRIPPER SERVO:    (for grippers: ) OPEN:600, CLOSED:2300 
  
  int outputs[5];
  
  outputs[0] = (500 + angles[0]*(2220-500)/180);
  outputs[1] = (750 + angles[1]*(1600-750)/90);
  outputs[2] = (830 + angles[2]*(1700-830)/90);
  outputs[3] = (660 + angles[3]*(2475-660)/180);
  
  
  // Note the format of the data being written to the Servo driver board should appear like so:
  //
  // EX.          #1P1500#2P1000#3P650T1000
  //
  // This specifies each motor number, its desired position in µs, and the total time that the 
  //  move should take in ms.
  
  Serial1.begin(115200);
  
  // Output values are written to the Servo driver
  Serial1.print("#0P");
  Serial1.print(outputs[0]);
  Serial1.print("#1P");
  Serial1.print(outputs[1]);
  Serial1.print("#2P");
  Serial1.print(outputs[2]);
  Serial1.print("#3P");
  Serial1.print(outputs[3]);
  Serial1.print("T");
  Serial1.println(duration);
  
  Serial1.end();
  
  // Since new outputs have been written, current configuration is updated
  currConfig[0] = angles[0];
  currConfig[1] = angles[1];
  currConfig[2] = angles[2];
  currConfig[3] = angles[3];
  
  updatePos(currPos, currConfig);
} 

void setTunings(int tunings[]){
  //  Argument to this function, tunings, is an array with signed integer values in μs specifying a shift 
  //  clockwise or counter clockwise which commands to the servo controller should be adjusted. This 
  //  allows some leway in mounting the Lynxmotion arm, allowing a slight adjustment here in setup rather than 
  //  recalibrating the entire range of the motor in the toConfiguration function.

  Serial1.print("#0PO");
  Serial1.print(tunings[0]);
  Serial1.print("#1PO");
  Serial1.print(tunings[1]);
  Serial1.print("#2PO");
  Serial1.print(tunings[2]);
  Serial1.print("#3PO");
  Serial1.print(tunings[3]);
  Serial1.print("#4PO");
  Serial1.println(tunings[4]);

}

void getConfiguration(float x, float y, float z, float *Configuration){
  // Function performs inverse kinematics to determine configuration necessary to position cutting point of claw 
  //  at point input to coordinates. Coordinates are stored at array *Configuration, which is recommended to be
  //  global array targetConfig[4]
  
  // Loads target position with coordinates
  targetPos[0] = x;
  targetPos[1] = y;
  targetPos[2] = z; 
  
  // Calculates base angle
  float baseAngle;
  baseAngle = (-atan(y/x))*RADTODEG;
  if(baseAngle<0){baseAngle+=180;}
 

  // Calculates the reach necessary to grasp cane
  float reach = sqrt(x*x+y*y);
  float alpha;
  
  // If cane is adequately far from base of robot, standard configuration is solved for
  if(reach >= 8)
  {
	  float lbw = sqrt(z*z+(reach*reach-2*L4*reach+L4*L4));
	  
	  if(z!=0){
		alpha = atan(z/(reach-L4));
		}
	  else{
		alpha = 0;
		}
		
	  // Calculates angles
	  float elbowAngle = 180*DEGTORAD - acos((L2*L2+L3*L3-lbw*lbw)/(2*L2*L3));
	  float shoulderpartangle = asin((L3/lbw)*sin(180*DEGTORAD-elbowAngle));
	  float shoulderAngle = alpha + shoulderpartangle;
	  float wristAngle = elbowAngle - shoulderAngle + 90*DEGTORAD;
	  // Stores angles
	  Configuration[0] = baseAngle;
	  Configuration[1] = RADTODEG*shoulderAngle;
	  Configuration[2] = RADTODEG*elbowAngle;
	  Configuration[3] = RADTODEG*wristAngle;
	  
	  // Inverse sine in shoulderpartangle has two possible solutions for the range of 0-180 of the shoulder angle.
	  //  The asin() function outputs only angles from -90 to 90, ignoring larger values. Over the close range of the operating
	  //  zone close to the arm, this causes incorrect angular solutions, and a check must be made using forward kinematics to verify
	  //  the legitimacy of the output solution.
	 
	  float ratio=0;
	  int c;
	  int fail = 0;
	  
	  // Forward kinematics check position output by previous calculations
	  updatePos(comparePos, Configuration);
	  
	  //Serial.println(comparePos[0]);
	  //Serial.println(comparePos[1]);  
	  //Serial.println(comparePos[2]);
	  
	  // Checks for a correct physical position
	  for(c=0; c<3; c++)
	  {
		 ratio = (comparePos[c])/(targetPos[c]);
		 if((targetPos[c] != 0 ) && (ratio > 1.005 || ratio < 0.995))
		 {
			fail = 1;
		 }	 
	  }
	  
	  // Recalculates using the alternate asin output.
	  if(fail == 1)
	   {
		 Serial.print("");
		//Serial.println("Config Failed, using alternative asin() output");
		float diffangle = (3.1415926/2) - shoulderpartangle;
		shoulderpartangle = diffangle + (3.1415926/2);
		float shoulderAngle = alpha + shoulderpartangle;
		float wristAngle = elbowAngle - shoulderAngle + 90*DEGTORAD;
		
		// Stores angles
		Configuration[0] = baseAngle;
		Configuration[1] = RADTODEG*shoulderAngle;
		Configuration[2] = RADTODEG*elbowAngle;
		Configuration[3] = RADTODEG*wristAngle;  
		
		
	   } 
  }
  // If cane is within 8 inches of robotic arm base, inward configuration is solved for
  if(reach < 8)
  {
	//Serial.println("Position is too close to base, so inward configuration is used.");
	
	float L5 = sqrt(L4*L4 + L3*L3);
	alpha = atan(z/reach);
	float beta = atan(L4/L3); // L4 may be changed to L4 - n, with n being the distance from standard cutting position along blade
	float dist = sqrt( x*x + y*y + z*z);
	
	float a = acos((L2*L2 + L5*L5 - dist*dist)/(2*L2*L5));
	
	float elbowAngle = 3.1415926 - beta - a;
	
	float shoulderpartangle = asin((L5*sin(a))/dist);
	float shoulderAngle = shoulderpartangle+alpha;
	
	float wristAngle = 0;
	
	// Stores angles
	Configuration[0] = baseAngle;
	Configuration[1] = RADTODEG*shoulderAngle;
	Configuration[2] = RADTODEG*elbowAngle;
	Configuration[3] = RADTODEG*wristAngle; 
	
	/* Serial.print("Reach is ");
	Serial.println(reach);
	Serial.print("Base angle is ");
	Serial.println(baseAngle);
	Serial.print("Shoulder angle is ");
	Serial.println(RADTODEG*shoulderAngle);
	Serial.print("Elbow angle is ");
	Serial.println(RADTODEG*elbowAngle);
	Serial.print("Alpha is ");
	Serial.println(RADTODEG*alpha);
	Serial.print("Wrist angle is ");
	Serial.println(RADTODEG*wristAngle);  */
 
	
	// Inverse sine in shoulderpartangle has two possible solutions for the range of 0-180 of the shoulder angle.
	//  The asin() function outputs only angles from -90 to 90, ignoring larger values. Over the close range of the operating
	//  zone close to the arm, this causes incorrect angular solutions, and a check must be made using forward kinematics to verify
	//  the legitimacy of the output solution.
	 
	float ratio=0;
	int c;
	int fail = 0;
	  
	// Forward kinematics check position output by previous calculations
	updatePos(comparePos, Configuration);
	  
	//Serial.println(comparePos[0]);
	//Serial.println(comparePos[1]);  
	//Serial.println(comparePos[2]);
	  
	// Checks for a correct physical position
	for(c=0; c<3; c++)
	{
		ratio = (comparePos[c])/(targetPos[c]);
		if((targetPos[c] != 0 ) && (ratio > 1.005 || ratio < 0.995))
		{
			fail = 1;
		}	 
	}
	
	if(fail == 1)
	{
		Serial.print("");
		Serial.println("Config Failed, using alternative asin() output");
		float diffangle = (3.1415926/2) - shoulderpartangle;
		shoulderpartangle = diffangle + (3.1415926/2);
		float shoulderAngle = alpha + shoulderpartangle;
		Configuration[1] = RADTODEG*shoulderAngle;
	 }
	
  }
  
}

void toCane(float x, float y, float z){
	
	// Robotic arm has been calibrated for radial positional accuracy on the plane comprised of the surface of the board
	//  emperically found to be z=-4 with respect to the coordinates of the base of the robotic arm
	// Any cane that is to be pulled must have its x and y coordinates transformed based on the calibration
	// The arm was calibrated in the following manner.
	//  	- Desired position is chosen. Ex: 8 inches radially from base of robotic arm
	//		- Input values for the robotic arm to reach (Ex. 8.3 radial inches) until the position is observed to be equal to that
	//           of the desired position first chosen
	//      - Necessary input to acheive desired output is recorded, and executed for every whole inch value from 8-21 inches
	//      - A regression tool was used to generate a 6th order polynomial, whose input is the desired position, and whose output
	//           is the necessary argued radial value to achieve the desired input
	//      - Coefficients of polynomial aproximation were defined as P1, P2, ... P7
	
	float r_pos = sqrt(x*x + y*y);
	float diff = calibrate(r_pos) - r_pos;
	
	float baseAngle = atan(y/x);
	
	//Serial.println(baseAngle);
	
	if(baseAngle<0)(baseAngle+=3.1415926);
	//Serial.println(baseAngle*RADTODEG);
	
	if(x != 0){
		x += diff * cos(baseAngle);
	}
	
	y += diff * sin(baseAngle);
	
	/* Serial.println(x);
	Serial.println(y);
	Serial.println(z); */
	
	toPoint(x,y,z);
}

void toPoint(float x, float y, float z){
 	// Solves for the configuration, then writes the configuration necessary to place the end
	//  effector at the argued coordinates in the argued amount of time.
	
	getConfiguration(x, y, z, targetConfig);
	
	duration = getDuration();
	toConfiguration(targetConfig, duration);
	
	delay(duration);
	
}


float calibrate(float r_distance){

  double x = r_distance;
  float newVal;

  newVal = P1*(pow(x, 6)) +P2*(pow(x, 5)) +P3*(pow(x, 4)) + P4*(pow(x, 3)) +P5*(pow(x, 2)) + P6*x +P7;

  return newVal;
}

void followLine(float point1[], float point2[], float waypointcount){
    // followLine causes the end effector to follow the straight line between two point arguments
	//  which allows the user to specify a particular path.
	
	
    // Straightline path between start and endpoints used to determine waypoint
    //  increment	
	float dx = (point2[0] - point1[0])/waypointcount;
	float dy = (point2[1] - point1[1])/waypointcount;
	float dz = (point2[2] - point1[2])/waypointcount;
	
	// Waypoint declared
	float waypoint[3]= {point1[0], point1[1], point1[2]};
	
	float i;
	
	for(i=0; i<waypointcount+1; i++){
		
		waypoint[0]+= dx;
		waypoint[1]+= dy;
		waypoint[2]+= dz;
		
		// Travels to waypoint
		toPoint(waypoint[0], waypoint[1], waypoint[2]);
	}
}

void facePoint(float x, float y, float z){
  // Calculates base angle necessary to face a point, and then does so, while retaining all other
  //  servo angles
  
  float baseAngle;
  baseAngle = (-atan(y/x))*57.2958;
  if(baseAngle<0){baseAngle+= 180;}
  
  currConfig[0] = baseAngle;
  toConfiguration(currConfig, 500);
  //printConfig();
  delay(500);
}

void cutSequence(float x, float y, float z, float dist){

  facePoint(x, y, z);
  
  for(int i=0; i<3; i++){prevPos[i]=currPos[i];}
  
  if(dist < 10)
  {
	  toPoint( x, y, z+6);
	  
	  targetPos[0] = x;
	  targetPos[1] = y;
	  targetPos[2] = z;
	  
	  followLine( currPos, targetPos, 20);
  }
  
  toCane(x, y, z);
  closeClaw();
  
  /* float baseAngle = atan(y/x);
  if(baseAngle<0){baseAngle+=3.1415926};
  if(x != 0){x += 2.5 * cos(baseAngle);}
  y += 2.5 * sin(baseAngle);
  
  toCane(x,y,z);   */
  
  targetPos[2] = currPos[2] + 5;
  toPoint(targetPos[0], targetPos[1], targetPos[2]);
  toPoint(prevPos[0], prevPos[1], prevPos[2]);
  depositItem();
  
}

void cutSequenceY(float x, float y, float z, float dist){
	
  facePoint(x, y, z);
  
  for(int i=0; i<3; i++){prevPos[i]=currPos[i];}
  
  if(dist < 10)
  {
	  toPoint( x, y, z+6);
	  
	  targetPos[0] = x;
	  targetPos[1] = y;
	  targetPos[2] = z;
	  
	  followLine( currPos, targetPos, 20);
  }
  
  toCane(x, y, z);
  closeClaw();
  
  float baseAngle = atan(y/x);
  if(baseAngle<0){baseAngle+=3.1415926;}
  if(x != 0){x += 2.5 * cos(baseAngle);}
  y += 2.5 * sin(baseAngle);
  
  toCane(x,y,z);   
  
  targetPos[2] = currPos[2] + 5;
  toPoint(targetPos[0], targetPos[1], targetPos[2]);
  toPoint(prevPos[0], prevPos[1], prevPos[2]);
  depositItem();
	
}

void toReady(void){
  
  targetConfig[0] = 90;
  targetConfig[1] = 135;
  targetConfig[2] = 135;
  targetConfig[3] = 90;

  toConfiguration(targetConfig, 500);
  openClaw();
}

void depositItem(void){

  targetConfig[0] = 190;
  targetConfig[1] = 150;
  targetConfig[2] = 137;
  targetConfig[3] = 90;
  
  toConfiguration(targetConfig, 750);
  delay(750);
  openClaw();
}

void toFetal(void){
  
  targetConfig[0] = 90;
  targetConfig[1] = 180;
  targetConfig[2] = 150;
  targetConfig[3] = 0;

  toConfiguration(targetConfig, 250);
}

void updatePos(float *Pos, float *Config){
	
	Pos[2] = L2*sin(DEGTORAD*(Config[1]))+ L3*sin(DEGTORAD*(Config[1]-Config[2]))+L4*sin(DEGTORAD*(Config[1]-Config[2]+Config[3]-90));
	float reach;
	reach = L2*cos(DEGTORAD*(Config[1]))+ L3*cos(DEGTORAD*(Config[1]-Config[2]))+L4*cos(DEGTORAD*(Config[1]-Config[2]+Config[3]-90));
	Pos[0] = -reach*cos(DEGTORAD*Config[0]);
	Pos[1] = reach*sin(DEGTORAD*Config[0]);
	
	/* Serial.println("End-effector Position is:");
	Serial.print('(');
	Serial.print(currPos[0]);
	Serial.print(',');
	Serial.print(currPos[1]);
	Serial.print(',');
	Serial.print(currPos[2]);
	Serial.println(')'); */
}

int getDuration(void){
	
	float dur;
	dur = (targetPos[0]-currPos[0])*(targetPos[0]-currPos[0]) + (targetPos[1]-currPos[1])*(targetPos[1]-currPos[1]) + (targetPos[2]-currPos[2])*(targetPos[2]-currPos[2]);
	dur = (double) sqrt(dur);
	dur = (int)(dur*1000/IPS);
	
	return dur;
	
}

// CLAW USAGE ----------------------------------------------------------------------------------------------------//

void closeClaw(void){
  
	Serial1.begin(115200);
	Serial1.print("#4P");
    Serial1.print(2100);
    Serial1.print("T");
    Serial1.println(100);
	delay(500);
	Serial1.end();
  
}

void openClaw(void){
	
	Serial1.begin(115200);
	Serial1.print("#4P");
    Serial1.print(1500);
    Serial1.print("T");
    Serial1.println(100);
	delay(500);
	Serial1.end();
	
}

// USER CONTROL ---------------------------------------------------------------------------------------------------//
void commandMode(void){	

	Serial.println("USER CONTROL MODE");
	Serial.println("Type and send one of the following numbers to issue a command.");
	Serial.print("\n");
	Serial.println("0 - Home Position");
	Serial.println("1 - Ready Position");
	Serial.println("2 - Enter Position");
	Serial.println("3 - Write to Servo");
	Serial.println("4 - Open Claw");
	Serial.println("5 - Close Claw");
	Serial.println("6 - Print Current Configuration    (DOES NOT REFLECT WRITES TO INDIVIDUAL MOTORS)");
	Serial.println("7 - Print Current Position         (DOES NOT REFLECT WRITES TO INDIVIDUAL MOTORS)");
	 
	int cmd;
	
	while(1){
		
		cmd = getCommand();
		
		//Serial.println(cmd);
		
		if(cmd==0){toFetal();}
		if(cmd==1){toReady();}
		if(cmd==2){
			Serial.print("\n");
			sendPosition();}
		if(cmd==3){
			Serial.println("Enter command in the format specified by LynxMotion documentation.");
			Serial.println("EXAMPLE:       #1P1500T1000");
			sendCommand();
		}
			
		if(cmd==4){openClaw();}
		if(cmd==5){closeClaw();}
		if(cmd==6){
			Serial.print("\n");
			printConfig();
		}
		if(cmd==7){
			Serial.print("\n");
			printPosition();
		}
		
		if(cmd>7){Serial.println("Please enter a Valid Command");}
			
	}
	
}


void sendCommand(void){
	// Calling sendCommand allows for the user to send the position of one servo motor of the format 
    //  described in toConfiguration() through the serial monitor window and on to the Servo driver board.	
	// 
	//  EXAMPLE:       #1P1500T1000
	//
	//  This example string to be sent through the serial monitor sets motor #1 to a position of 1500µs
	//  over a duration of 1000 ms (1 sec).

	while(Serial.available() == 0){}
	
	// Checks for incoming data from Serial Monitor, allows space for one motor write command
	//  within 
      char Command[15];
      int index = 0;
      char inChar;
     
      while(Serial.available()> 0){
    
          if(index < 14) // One less than the size of the array
          {
            inChar = Serial.read(); // Read a character
            Command[index] = inChar; // Store it
            index++; // Increment where to write next
            Command[index] = '\0'; // Null terminate the string
          }
          delay(3);
        }
		
	  // Pushes data from Command buffer to servo controller along Serial1
      if(index>0){
          for(int i=0; i<index; i++){
               Serial1.print(Command[i]);
            }
			Serial1.println(Command[index]);
        }
		
		//Serial1.println("#0P1500T1000");

}

void sendPosition(void){
	// Prompts User for coordinates of point to which cutter will be positioned

	float point[3]; 
	
	Serial.println("Enter coordinates as prompted:");
	
	Serial.print("x-coordinate:        ");
	point[0] = getCoordinate();
	Serial.println(point[0]);
	
	Serial.print("y-coordinate:        ");
	point[1] = getCoordinate();
	Serial.println(point[1]);
	
	Serial.print("z-coordinate:        ");
	point[2] = getCoordinate();
	Serial.println(point[2]);
	Serial.println("");
	
	facePoint(point[0], point[1], point[2]);
	toCane(point[0], point[1], point[2]);
	
}

float getCoordinate(void){
	// Retrieves coordinate sent over the Serial Comm. interface
	
	while(Serial.available() == 0){}
	
	char coordinate[6];
    int index = 0;
    char inChar;
     
    while(Serial.available()> 0){
    
        if(index < 6) // One less than the size of the array
        {
          inChar = Serial.read(); // Read a character
          coordinate[index] = inChar; // Store it
          index++; // Increment where to write next
          coordinate[index] = '\0'; // Null terminate the string
        }
        delay(3);
    }
	
	return atof(coordinate);
}

int getCommand(void){
	// Retrieves coordinate sent over the Serial Comm. interface
	
	while(Serial.available() == 0){}
	
	char command[2];
    int index = 0;
    char inChar;
     
    while(Serial.available()> 0){
    
        if(index < 2) // One less than the size of the array
        {
          inChar = Serial.read(); // Read a character
          command[index] = inChar; // Store it
          index++; // Increment where to write next
        }
        delay(3);
    }
	
	return atoi(command);
}

void printConfig(void){
	// Prints to the Serial monitor the current configuration of the robotic arm
    //  *NOTE* : This configuration is not updated when commands are sent through the serial 
    //   monitor, but only done when getConfiguration is called.	
	
	Serial.println(" CURRENT CONFIG ");
	Serial.print(" Base Angle:       ");
	Serial.println(currConfig[0]);
	Serial.print(" Shoulder Angle:   ");
	Serial.println(currConfig[1]);
	Serial.print(" Elbow Angle:      ");
	Serial.println(currConfig[2]);
	Serial.print(" Wrist Angle:      ");
	Serial.println(currConfig[3]);
	
}

void printPosition(void){
	// Prints to the Serial monitor the current position of the the midpoint of the cutter
	
	Serial.println(" CURRENT POSITION");
	Serial.print(" [");
	Serial.print(currPos[0]);
	Serial.print(",");
	Serial.print(currPos[1]);
	Serial.print(",");
	Serial.print(currPos[2]);
	Serial.println("]");
	
}

