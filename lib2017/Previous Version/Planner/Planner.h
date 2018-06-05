#ifndef PLANNER   /* Include guard */
#define PLANNER

// Includes caneCoordinates to receive, parse, and sort data
// Includes SCC_Driver to issue commands to the robotic arm
// Includes motordriving to drive robot to desired x-positions along side of row

// Library is used to plan and execute the pulling of canes from a zone

  // Competition Sequences
  void initialize(void);
  void zoneSequenceGreen(void);
  
  // Obstruction Checks
  void checkObstructions(void);
  int isObstructed(int cane);
  int pullSolver(void);
  void caneSolver(int cane, float xmin, float xmax);
  
  // Pulling sequences 
  //	GREEN
  int cutUnblocked(void);
  void cutBlocked(void);
  
  // Support functions
  float getMin(float *x, int count);
  float getMax(float *x, int count);
  void startClock(void);
  void stopClock(void);
  
  // Camera mast functions
  void initializeStepper(void);
  void raiseRGB(void);
  void lowerRGB(void);
  
  // Chassis Drive Functions
  
  
#endif