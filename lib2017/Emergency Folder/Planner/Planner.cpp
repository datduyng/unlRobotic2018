#include "Planner.h"
#include "SCC_Driver.h"
#include "caneCoordinates.h"
#include "Arduino.h"
//#include "BasicStepperDriver.h"
#include "Driving.h"
#include <RaisingCamera.h>

// OBSTRUCTION CHECKING DEFINITIONS
	// Sets the maximum tolerable distance of yellow canes to green cane pull-pathway
	#define MAX 1.5
	// Sets the resolution of images checkd for unblocked canes
	#define solResolution 0.25
	#define REACH 20.5
	
// DRIVING DEFINITIONS

// CAMERA MAST DEFINITIONS
	// Motor steps per revolution. Most steppers are 200 steps or 1.8 degrees/step
	#define MOTOR_STEPS 200
	#define SRPM 10

	// All the wires needed for full functionality using Camera Mast
	#define DIR 8
	#define STEP 9

	// Since microstepping is set externally, make sure this matches the selected mode
	// 1=full step, 2=half step etc.
	#define MICROSTEPS 1

	// 2-wire basic config, microstepping is hardwired on the driver
	//BasicStepperDriver stepper(MOTOR_STEPS, DIR, STEP);
	
// GENERAL DEFINITIONS
	extern float targetConfig[4];
	float tstart;
	float wayPoints[3] = {0,0,0};
	
//======================================================================================================================//

// COMPETITION SEQUENCES
void initialize(void)
{
	pinMode(28, OUTPUT);
	pinMode(29, OUTPUT);
	pinMode(30, OUTPUT);
	digitalWrite(28, LOW);
	digitalWrite(29, LOW);
	digitalWrite(30, LOW);
	dinit();
	cinit();
	downRGB();
	//initializeStepper();
}

void competitionSequence(void)
{
	initialize();
	
	// CLEARS GREEN ROW
	driveto(5.2 -global_x);
	zoneSequenceGreen();
	driveto(17.2 -global_x);
	zoneSequenceGreen();
	driveto(29.2-global_x);
	zoneSequenceGreen();
	driveto(41.2-global_x);
	zoneSequenceGreen();
	driveto(53.2-global_x);
	zoneSequenceGreen(); 
	
	// ROBOT TURNS AROUND TO TRAVEL PARALLEL TO YELLOW ROW
	
	// CLEARS YELLOW ROW
	
}

/* 
void zoneSequenceYellow(void)
{
	// Raises RGB camera mast
	upRGB();
	delay(1000);
	
	// Sends command to Raspberry Pi to initiate computer vision algorithms
	Serial.println("Go");
	
	// Waits for a data package from the Raspberry Pi, receives and parses cane position data
	receivePackage();
	
	// Retracts RGB camera mast  
	downRGB();
	
	// Moves robot such that robotic arm is free to pull row
	driveto(5);
	updateCanes(5);
	
	// Calculates the radial distance from base of robot and sorts green and yellow canes in zone by distance from arm
	addDist(); 
	sortCanesets();
	
	// Pulls yellow canes in order of proximity
	
	
}

*/

void zoneSequenceGreen(void)
{
	
	// Raises RGB camera mast
	upRGB();
	delay(1000);
	
	// Sends command to Raspberry Pi to initiate computer vision algorithms
	Serial.println("Go");
	
	// Waits for a data package from the Raspberry Pi, receives and parses cane position data
	receivePackage();
	
	// Retracts RGB camera mast  
	downRGB();
	
	// Moves robot such that robotic arm is free to pull row
	driveto(5);
	updateCanes(5);
	
	// Calculates the radial distance from base of robot and sorts green and yellow canes in zone by distance from arm
	addDist(); 
	sortCanesets();
	
	// Checks for obstructions to green canes
	checkObstructions();
	
	// Executes cutting of unblocked Canes and returns the number that have been pulled
	if( (cutUnblocked()) != G.count)
	{
		// If some canes were unpulled, pullSolver determines the nearest position of the robot necessary 
		//    to pull each blocked cane, and returns the number ready to pull
		if( (pullSolver()) > 0)
		{
			 // Any canes ready to pull are driven to and pulled
			 cutBlocked();
		}
	}
		
	toFetal();
}

// OBSTRUCTION CHECKS
void checkObstructions()
{
	Serial.println("Checking for obstructions to green canes.");
	int i; 
	
	// Iterates through each green cane present
	for(i = 0; i < G.count; i++)
	{
		// Each cane is checked, and, if blocked or out of range, the blocked flag of the cane's control register is raised
		if( (isObstructed(i)) == 1 ){
			G.Status[i] |= 0x04;
			G.xrob[i] = 13;
		}
	}
}

int isObstructed(int cane){
	
	int j;
	
	float prox=0;
	int blocked = 0;
	
	// If green cane is within pulling distance
	if( G.dist[cane] < REACH)
	{		
		// Checks whether any yellow canes are blocking the green cane
		for(j = 0; j < Y.count; j++)
		{
			// Considers only yellow canes closer than current green cane
			if(Y.dist[j] < G.dist[cane])
			{
				// Yellow cane distance from green line of action is calculated
				prox = abs((G.x[cane]*(-Y.y[j]) + (Y.x[j])*(G.y[cane]))/G.dist[cane]);
				//Serial.println(prox);
				
				// If proximity is within maximum setting, cane is blocked
				if( prox < MAX)
				{
					blocked = 1;
					/* Serial.print("Green cane ");
					Serial.print(cane+1);
					Serial.print(" is obstructed by a yellow at a distance of ");
					Serial.print(prox);
					Serial.println(" inches."); */
				}
			}
		}
	}
	// If green cane is not within pulling distance of the arm
	else
	{
		blocked = 1;
		/* Serial.print("Green cane ");
		Serial.print(cane+1);
		Serial.println(" is out of range of the robot."); */
	}
		
		return blocked;	
	}
	
	// Determines the x position of the robot necessary to pull each cane that is currently blocked, if such a position exists.
//	 This displacement is then stored as xrob in the green cane structure for each such cane. To accomplish this, global 
//   coordinates of each cane are repeatedly manipulated, and initial values are restored at the end of the function in order
//   to ensure current dataset reflects the actual position of the robot.
int pullSolver(void){
	
	int ready = 0;
	float xmin;
	float xmax;
	int i;
	
	// Saves the initial x positions of the canes in this zone
	for(i=0; i<G.count; i++){
		G.x_init[i] = G.x[i];
	}
	for(i=0; i<Y.count; i++){
		Y.x_init[i] = Y.x[i];
	}
	
	// Determines bounds of the locations of canes relative to the robot
	xmin = getMin(G.x, G.count);
	if(getMin(Y.x, Y.count) < xmin){xmin = getMin(Y.x, Y.count);}
	
	xmax = getMax(G.x, G.count);
	if(getMax(Y.x, Y.count) > xmax){xmax = getMax(Y.x, Y.count);}

	// Iterates through each green cane 
	for(i=0; i<G.count; i++)
	{
		// Considers only canes that are blocked
		if( (G.Status[i] &= 0x04) != 0)
		{
			caneSolver(i, xmin, xmax);
		}	
	}
	
	// Restores the canes to their initial x positions of the canes in this zone
	for(i=0; i<G.count; i++){
		G.x[i] = G.x_init[i]; 
	}
	for(i=0; i<Y.count; i++){
		Y.x[i] = Y.x_init[i];
	}
	
	// Restores correct distance values to canes 
	addDist();
	
	// Prints results from solver:
	for(i=0; i<G.count; i++)
	{
		// Prints xrob positions necessary for pulling ready canes.
		if( (G.Status[i] & 0x02) != 0)
		{
			Serial.print("Green cane ");
			Serial.print(i+1);
			Serial.print(" is ready to pull. Robot must move ");
			Serial.print(G.xrob[i]); 
			Serial.println(" along the length of the zone.");
			ready += 1;
		}
		
		// Prints id number of canes for which a pulling solution failed.
		if( (G.Status[i] & 0x08) != 0)
		{
			Serial.print("Green cane ");
			Serial.print(i+1);
			Serial.println(" has failed. No solution for pulling position was found.");
		}
	}

	return ready;
}

// Given the minimum and maximum x positions of all canes relative to the robot, caneSolver determines the x position
//    of the robot in the range bounded by these terms in which the green cane in question is unobstructed thus "finding a clear path"
void caneSolver(int cane, float xmin, float xmax){
	
	// Value used to fill the xrob value of each cane, indicating the necessary position to which the robot must move in order for 
	//    the cane to be pulled
	float robOffset;
	int i;
	
	// Iterates through .25" increments of x position surrounding canes, checking for an unblocked condition at each
	for(robOffset = (xmin-1.5); robOffset <= (xmax+1.5); robOffset += solResolution)
	{
		//Serial.print("Checking at robot position x = ");
		//Serial.println(robOffset);

		// X value of all green and yellow canes is shifted by the amount that would be observed by the robot if it moved 
		//   a distance along the length of the zone equal to robOffset
		for(i=0; i<G.count; i++)
		{
			G.x[i] = G.x_init[i] - robOffset;
		}
		for(i=0; i<Y.count; i++)
		{
			Y.x[i] = Y.x_init[i] - robOffset;
		} 
		
		// Updates distances to each cane for current robOffset (possible pull location)
		addDist();
		
		// Checks whether green cane is obstructed from this position, and, if not, stores robOffset under xrob, sets the 
		//    "ready" status flag, and breaks the loop.
		if( isObstructed(cane) == 0 )
		{
			if( fabs(robOffset) < fabs(G.xrob[cane])){ G.xrob[cane] = robOffset;}
			G.Status[cane] |= 0x02;
		}
	}
	
	// Checks whether cane is ready (whether a solution has been found);
	if( (G.Status[cane] & 0x02) == 0)
	{
		// if not, cane is set to "failed"
		G.Status[cane] |= 0x08;
	}
}


// PULLING SEQUENCES

// Cuts all unobstructed green canes at current robot position and returns the number that have been cut
int cutUnblocked(void){

  int cut = 0;
  toReady();
  
  for(int j=0; j<G.count; j++){

	// Iterates through each green cane. All those canes unobstructed are cut and deposited
	if( (G.Status[j] & 0x04) == 0)
	{
		Serial.print("Retrieving Cane ");
		Serial.println(j+1);
		Serial.print("\n");
		cutSequence(G.x[j] ,G.y[j] ,G.z[j], G.dist[j]);
		
		// Marks cane as pulled
		G.Status[j] |= 0x01;
		cut += 1;
	}
	else
	{
		Serial.print("Cane ");
		Serial.print(j+1);
		Serial.println(" is obstructed, and is skipped.\n");
	}
  }
  
  return cut;
}

// Travels to cutting location of all currently blocked canes in order of their proximity to robot and removes them
void cutBlocked(void)
{
	toReady();
	
	wayPoints[0] = 0;
	wayPoints[1] = 0;
	wayPoints[2] = 0;
	
	int i;
	int j;
	int index;
	float insert;
	
	// First sorts the waypoints for pulling blocked but ready canes from nearest to farthest away
	//    from the current position of the robot
	/* Serial.println(G.xrob[0]);
	Serial.println(G.xrob[1]);
	Serial.println(G.xrob[2]); */
	for(i=0; i<G.count; i++)
	{
		wayPoints[i] = G.xrob[i];
	}
  
  for(i = 1; i<3; i++)
  {
    index = i;
    insert = wayPoints[i];

    while(index > 0 && wayPoints[index-1] > insert)
    {
        wayPoints[index] = wayPoints[index - 1];
        index--;
    }
    
    if(index != i)
    {
        wayPoints[index] = insert;
    }   
     
  }
	/* Serial.println(wayPoints[0]);
	Serial.println(wayPoints[1]);
	Serial.println(wayPoints[2]); */
	
	// Iterates through sorted waypoints
	for(i=0; i<3; i++)
	{
		// Checks each green cane to identify the one ready to pull
		for(j=0; j<G.count; j++ )
		{
			// Compares xrob values to identify the cane, and verifies using ready status
			if( (G.xrob[j] == wayPoints[i]) && ((G.Status[j] & 0x02) !=0 ))
			{
				
				// Once the cane is determined, it is driven to and then pulled
				//////////////// DRIVES TO XROB POSITION //////////////////////
				driveto(G.xrob[j]);
				
				Serial.print("Robot has driven ");
				
				if(G.xrob[j] > 0)
				{
					Serial.print("forward ");
					Serial.print(G.xrob[j]);
				}
				
				if(G.xrob[j] < 0)
				{
					Serial.print(" backward ");
					Serial.print(-G.xrob[j]);
				}
				
				Serial.print(" inches, and is now ready to pull cane ");
				Serial.print(j+1);
				Serial.println(" .");
				
				updateCoordinates(G.xrob[j]);
				addDist();
				
				/* // Updates the values of each of the waypoints in the queue to reflect move
				for(k=0; k<3; k++)
				{
					wayPoints[k] = wayPoints[k] - G.xrob[j];
				}
				// Updates the positions of each of the yellow cane coordinates to reflect move
				for(k=0; k<Y.count; k++)
				{
					Y.x[k] = Y.x[k] - G.xrob[j];
				} 
				
				// Updates cane coordinates to reflect move robot has made after travelling a distance of xrob along the zone
				for(k=0; k<G.count; k++)
				{
					G.x[k] = G.x[k] - G.xrob[j];
					
					// All xrob values for currently ready canes are also incremented by this amount
					if( (G.Status[k] & 0x02) != 0)
					{
						G.xrob[k] -= G.xrob[j];
					}
				} */
				
				// Cane is pulled
				Serial.print("Retrieving Cane ");
				Serial.println(j+1);
				
				cutSequence(G.x[j], G.y[j], G.z[j], G.dist[j]);
				
				// Cane status is updated to "pulled"
				G.Status[j] |= 0x01;
				// Ready status is lifted
				G.Status[j] &= ~0x02;
				
				break;
			}
		}	
	}
	
}

void updateCoordinates(float x_val)
{
	int k;
	
	// Updates the values of each of the waypoints in the queue to reflect move
	for(k=0; k<3; k++)
	{
		wayPoints[k] = wayPoints[k] - x_val;
	}
	
	// Updates the positions of each of the yellow cane coordinates to reflect move
	for(k=0; k<Y.count; k++)
	{
		Y.x[k] = Y.x[k] - x_val;
	} 
	
	// Updates cane coordinates to reflect move robot has made after travelling a distance of xrob along the zone
	for(k=0; k<G.count; k++)
	{
		G.x[k] = G.x[k] - x_val;
		
		// All xrob values for currently ready canes are also incremented by this amount
		if( (G.Status[k] & 0x02) != 0)
		{
			G.xrob[k] -= x_val;
		}
	}
	
}

void updateCanes(float x_val)
{
	int k;
	
	// Updates the positions of each of the yellow cane coordinates to reflect move
	for(k=0; k<Y.count; k++)
	{
		Y.x[k] = Y.x[k] - x_val;
	} 
	
	// Updates cane coordinates to reflect move robot has made after travelling a distance of xrob along the zone
	for(k=0; k<G.count; k++)
	{
		G.x[k] = G.x[k] - x_val;
	}
	
}

// Returns minimum value of arrays x of size count
float getMin(float *x, int count)
{
	float xmin = x[0];
	int i;
	for(i=0; i<count; i++)
	{
		if(x[i] < xmin)
		{
			xmin = x[i];
		}
	}
	
	return xmin;
}

// Returns maximum value in array x of size count
float getMax(float *x, int count)
{
	float xmax = x[0];
	int i;
	for(i=0; i<count; i++)
	{
		if(x[i] > xmax)
		{
			xmax = x[i];
		}
	}
	
	return xmax;
}

// TIMER FUNCTIONS
void startClock(void)
{
	tstart = millis();
}

void stopClock(void)
{
   // Reports the time elapsed since clock was started
   float totaltime = millis()-tstart;
   totaltime = totaltime/1000;
   Serial.print(totaltime);
   Serial.println(" seconds elapsed.");
}

