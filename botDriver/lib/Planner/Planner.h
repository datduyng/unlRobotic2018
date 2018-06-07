/**
 * Author Dat Nguyen
 * Date: 6/4/18
 * this library will help perform sequences and plan
 * the functionality of the robot.
 *Sequence:
 * Drive.
 * raise camera Arm Right.
 * get coordinates
 * rotate camera Arm around.
 * get caneCoordinates
 * perform relativeSwap()
 * store both coordinate in a global variable
 * robot Arm progress. 
 * repeat nth time until reach nth distance.
 *

#ifndef PLANNER   /* Include guard */
#define PLANNER

// Includes caneCoordinates to receive, parse, and sort data
// Includes SCC_Driver to issue commands to the robotic arm
// Includes motordriving to drive robot to desired x-positions along side of row

// Library is used to plan and execute the pulling of canes from a zone

  // Competition Sequences
  void initialize(void);
  void zoneSequenceGreen(void);
  void competitionSequence(void);

  // Obstruction Checks
  void checkObstructions(void);
  int isObstructed(int cane);
  int pullSolver(void);
  void caneSolver(int cane, float xmin, float xmax);
  void updateCoordinates(float x_val);
  void updateCanes(float x_val);

  // Pulling sequences
  //	GREEN
  int cutUnblocked(void);
  void cutBlocked(void);

  // Support functions
  float getMin(float *x, int count);
  float getMax(float *x, int count);
  void startClock(void);
  void stopClock(void);

#endif
