// TITLE : SCC_Driver
// AUTHOR: Lukas Renker

//  PURPOSE: This library is to facilitate easy use of the Lynxmotion AL5D, 4 DOF
//  robotic arm. 
//
//  The first category of functions allow for the conversion of convenient angle arguments to the format
//  necessary to write to the SCC-32 servo driver board (toConfiguration). Function getConfiguration
//  performs inverse kinematics to write the arm to a specific end-effector position. The two are 
//  in sequence using the toPoint function, for quick use. The followLine function breaks down the 
//  path between two points, and it moves the end effector in a straight line path from one to 
//  the next. 
//  
//  In order to facilitate troubleshooting and calibration, the sendCommand and printConfig functions 
//  allow for sending and receiving of coordinates through the serial monitor.

#ifndef SCC_DRIVER_H   /* Include guard */
#define SCC_DRIVER_H

// Robotic arm link lengths
#define L2 7.375
#define L3 9.375
#define L4 4.25

// Scaling coefficients used in polynomial regression for calibration of arm

// WHEN CUTTER IS IN USE
#define P1  -0.00000783670
#define P2  0.000667355
#define P3  -0.0233472
#define P4  0.431456
#define P5  -4.47091
#define P6  25.7167
#define P7  -55.9275 

// WHEN CLAW IS IN USE
//#define P1  -0.0000101049
//#define P2  0.000815521
//#define P3  -0.0269852
//#define P4  0.468593
//#define P5  -4.50169
//#define P6  23.5609
//#define P7  -44.8052



// Generic constants
#define RADTODEG 57.295779513
#define DEGTORAD (1/57.295779513)

// Speed of arm 
#define IPS 5

extern float currConfig[4];
extern float targetConfig[4];
extern float currPos[3];
extern float targetPos[3];


// ----------------------------------FUNCTIONS------------------------------------//

// Arm Positioning and Configuration
void getConfiguration(float x, float y, float z, float *Configuration);
void toConfiguration(float angles[], int duration);
void toPoint(float x, float y, float z);
void toCane(float x, float y, float z);
float calibrate(float r_distance);
void followLine(float point1[], float point2[], float waypointcount);
void facePoint(float x, float y, float z);
void setTunings(int tunings[]);
void updatePos(float *Pos, float *Config);
void cutSequence(float x, float y, float z, float dist);
void cutSequenceY(float x, float y, float z, float dist);
void toReady(void);
void depositItem(void);
void toFetal(void);
int getDuration(void);

// Claw Useage
void openClaw(void);
void closeClaw(void);

// ------------------------------------User Control-------------------------------//
void commandMode(void);
void sendCommand(void);
void printConfig(void);
void printPosition(void);
void sendPosition(void);
float getCoordinate(void);
int getCommand(void);


#endif
