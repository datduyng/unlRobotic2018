#ifndef INVKINE_H 
#define INVKINE_H

/* Include Library  */
#include "Arduino.h"
#include "math.h"
#include "stdlib.h"
#include "stdint.h"

/* Define Libary Constant	*/
/* Choose either English Unit or Metric	*/
//#define UNIT_ENGLISH 0
#define UNIT_METRIC	1
#define DEGTORAD	(M_PI/180.0f)		// for conversion from degree to radian
#define RADTODEG	(180.0f/M_PI)		// for conversion from radian to degree	


/* Define typedef struct*/
struct point_t{
	float x;
	float y;
	float z;
};

struct point_mm_t{
	int16_t x;
	int16_t y;
	int16_t z;
};

struct joint_t{
	float azimuth;
	float shoulder;
	float elbow;
	float wrist;
};

struct joint_mdeg_t{
	int16_t azimuth;
	int16_t shoulder;
	int16_t elbow;
	int16_t wrist;
};



/*	Function Prototype	*/
uint8_t forwardKinematic( joint_t *js, point_t *pt );
uint8_t inverseKinematic ( point_t *point , joint_t *js );
/*	Utilities Function	*/
uint8_t arm_checkBounds( point_t point );
uint8_t asin_checkbounds ( float x );
uint8_t acos_checkbounds ( float x );

#endif

