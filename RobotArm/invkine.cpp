/********************************************************************************************
*	Servo Motors have different standard control signals. Converting input angles to servo signals
*   requires control singal range information for each servo in the Lynxmotion AL5D 4-DOF arm.
*  
*   (Collected from datasheets)
*   AZIMUTH SERVO:	   HS-475HB     900-2100 µs : 200 degrees
*   SHOULDER SERVO:    HS-805BB     556-2420 μs : 199.5 degrees
*   ELBOW SERVO:       HS-755HB     556-2410 μs : 202 degrees
*   WRIST SERVO:       HS-645MG     553-2520 μs : 197 degrees
*   GRIPPER SERVO:     HS-422       1500 μs     : neutral
*  
*   Emperically determined in operation (neglecting the use of the base-to-elbow spring)
*   AZIMUTH SERVO:       [0-180, 580-2300µs]
*   SHOULDER SERVO    [0-90,  750-2375µs]
*   ELBOW SERVO       [0-90,  830-1700µs] 
*   WRIST SERVO       [0-180, 660-2475µs]
*   GRIPPER SERVO:    (for grippers: ) OPEN:600, CLOSED:2300 
*********************************************************************************************/
  
/*	Attach Libraries	*/
#include "invkine.h"
#include "Arduino.h"
#include "math.h"
#include "stdlib.h"
#include "stdint.h"

/* Declare Global Variables	*/


/*	Declare File Variables	*/

#ifdef UNIT_METRIC
	#define L2	(int32_t)146		// Shoulder to Elbow Linkage	
	#define L3	(int32_t)236		// Elbow to Wrist Linkage
	#define L4	(int32_t)115		// Wrist to grabbing point distance
#endif

/*
point_mm_t point;
joint_t currconfig;
*/

/*	Functions	*/

/*
	Function: arm_checkBounds
	Param	: point_t (cartesian coordinates)
	Return	: flag, 0 for success, 1 for failure
	Info	: Make sure the given coordinates is reachable
*/

uint8_t arm_checkBounds( point_t *point ){
	float radial_reach;
	uint8_t status = 0;
	int32_t sq1, sq2, sq3, sum;
	sq1 = (int32_t) point->x * (int32_t) point->x;
	sq2 = (int32_t) point->y * (int32_t) point->y;
	sq3 = (int32_t) point->z * (int32_t) point->z;
	sum = sq1 + sq2 +sq3;
	radial_reach = sqrt(sum);
	
	static int const MAX_REACH = L2 + L3 + L4;
	
	if ( radial_reach > MAX_REACH )	return 1;
	
	return status;
}

/**
	Function: forwardKinematic
	Param	: joint_t (joint space configuration)
	Return	: flag
	Info	: Use forward kinematic to compute the arm reach
**/

uint8_t forwardKinematic( joint_t *js, point_t *pt ){
	point_t local;
	uint8_t flag = 0;
	
	
	float radian0, radian1, radian2, radian3, radius;
	
	radian0 = js->azimuth * DEGTORAD;
	radian1 = js->shoulder * DEGTORAD;
	// the elbow joint angle is from full extend (0deg) to full contract (180deg)
	radian2 = radian1 - (js->elbow) * DEGTORAD;
	radian3 = radian2 + js->wrist * DEGTORAD - M_PI/2;
	
	radius = L2 * cos(radian1) + L3 * cos(radian2) + L4 * cos(radian3);
	
	local.y = radius * sin(radian0);
	local.x = radius * cos(-radian0);
	local.z = L2 * sin(radian1) + L3 * sin(radian2) + L4 * sin(radian3);
	
	pt = &local;
	
	return flag;
}

/*
	Function: inverseKinematic
	Param	: point_t (cartesian coordinates)
	Return	: flag
	Info	: Compute the joint space config for given cartesian coordinates
*/
uint8_t inverseKinematic ( point_t *point , joint_t *js ){
	
	joint_t local;
	uint8_t ret = 0;
	float sum, sum2, rho2, radius;
	
	/* 
		Compute some constant or variables
	*/
	
	sum = (float) (point->x * point->x + point->y * point->y);	/*	sum is the square of the projection on XY plane	*/
	static float const cache0 = (float)(L4 * L4);
	sum2 = sum + cache0 - 2 * sqrtf(sum) * L4;	/*	sum2 is the square of projection minus the last link	*/
	radius = sqrtf(sum2);	/* radius is the projection onto XY plane	*/
	sum2 += point->z * point->z;
	rho2 = sqrtf(sum2);	/* rho2 is the straight line to the wrist joint	*/
	
	/* Unused	*/
	//radius = sqrtf(sum);	
	//sum += point.z * point.z;	
	//rho = sqrtf(sum);		
	
	
	local.azimuth = atan2( point->x, point->y ) * RADTODEG;
	
	float num, ratio, complementary;
	
	/*	Cosine Law	*/
	static float const cache1 = (float) (L2 * L2 + L3 * L3);
	num = cache1 - sum2;
	static float const denom = (float) (2 * L2 * L3);
	ratio = num/denom;
	if( !acos_checkbounds(ratio) ){
		complementary = acos( ratio );
	}else{
		// Something bad happens
		ret = 1;
		return ret;
	}
	
	local.elbow = (M_PI - complementary) * RADTODEG;
	
	float alti, alti2, ratio2;
	alti = atan2( point->z,  radius);
	/*
	if ( complementary > M_PI/2 ){
      complementary-= M_PI;
    }
	*/
	ratio2 = L3 * sin(complementary) / rho2;
	if ( !asin_checkbounds(ratio2) ){
		alti2 = asin(ratio2);
	}else{
		// Something bad happens
		ret = 1;
		return ret;
	}
	
	/*	Inverse Sine return -pi/2 to pi/2, shifting it into 0 to pi/2	*/
	if (alti2 < 0 ){
		alti2 += M_PI;
	}
	
	local.shoulder = ( alti + alti2 ) * RADTODEG;
	local.wrist = local.elbow - local.shoulder + 90.0f;
	
	js = &local;
	
	return ret;
}

/*
	Function: asin_checkbounds 
	Param	: x
	Return	: Flag
	Info	: To ensure the input to inverse sine is within domain
			  [-1.0, 1.0]
*/
uint8_t asin_checkbounds ( float x ){
	uint8_t ret = 0;
	
	if ( x < -1.0f || x > 1.0f ){
		ret = 1;
	}
	return ret;
}

/*
	Function: acos_checkbounds 
	Param	: x
	Return	: Flag
	Info	: To ensure the input to inverse cosine is within domain
			  [-1.0, 1.0]
*/
uint8_t acos_checkbounds ( float x ){
	
	
	if ( x < -1.0f || x > 1.0f ){
		return 1;
	}
	return 0;
}
	
	
