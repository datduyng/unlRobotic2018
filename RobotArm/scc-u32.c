#include "scc-u32.h"
#include "invkine.h"

#include "Arduino.h"
#include "math.h"
#include "stdlib.h"
#include "stdint.h"


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
*   AZIMUTH SERVO:    [0-180, 580-2300µs]
*   SHOULDER SERVO    [0-90,  750-2375µs]
*   ELBOW SERVO       [0-90,  830-1700µs] 
*   WRIST SERVO       [0-180, 660-2475µs]
*   GRIPPER SERVO:    (for grippers: ) OPEN:600, CLOSED:2300 
*
*	All servos' neutral point are at 1500 µs
*********************************************************************************************/


/* file scope variable		*/
#define BUFFER_SIZE	128
static char msg[BUFFER_SIZE] = {};
static int16_t baudrate;


/* 
	Function: conversion
	Param	: joint_t, as_t
	Return	: flag
	Info	: convert joint space to actuator space
			  Note that the argument is the pointer to the struct	
*/
uint8_t as_conversion( joint_t *js, as_t *as ){
	uint8_t status = 0;
	as_t local = {};
	
	static uint16_t const AZIMUTH_OFFSET = 500;
	static uint16_t const SHOULDER_OFFSET = 750;
	static uint16_t const ELBOW_OFFSET = 830;
	static uint16_t const WRIST_OFFSET = 660;
	
	static uint16_t const AZIMUTH_SCALER = (uint16_t) ((float) (2100-900))/200.0f;
	static uint16_t const SHOULDER_SCALER = (uint16_t) ((float) (2420-556)/199.5f;
	static uint16_t const ELBOW_SCALER = (uint16_t) ((float) (2410-556))/202.0f;
	static uint16_t const WRIST_SCALER = (uint16_t) ((float) (2520-553))/197.0f;
		
	local.azimuth = js->azimuth * AZIMUTH_SCALER + AZIMUTH_OFFSET;
	local.shoulder = js->shoulder * SHOULDER_SCALER + SHOULDER_OFFSET;
	local.elbow = js->elbow * ELBOW_SCALER + ELBOW_OFFSET;
	local.wrist = js->wrist * WRIST_SCALER + WRIST_OFFSET;
	
  	as->azimuth = local.azimuth;
	as->shoulder = local.shoulder;
	as->elbow = local.elbow;
	as->wrist = local.wrist;
	
	return status;
}

/*	
	Function: joint space boundcheck	
	Param	: joint_t struct
	Return	: flag, 0 for pass, 1 for error
	
*/
uint8_t js_constain (joint_t *js){
	uint8_t flag = 0;
	
	js->azimuth = fconstrain(js->azimuth, 0.0f, 180.0f);
	js->shoulder = fconstrain(js->shoulder, 0.0f, 180.0f);
	js->elbow = fconstrain(js->elbow, 0.0f, 180.0f);
	js->wrist = fconstrain(js->wrist, 0.0f, 180.0f);
	
	return flag;
}



/* sending off servo cmd	*/
uint8_t setConfiguration ( as_t *as ){
	uint8_t flag = 0;
	
	snprintf(msg, BUFFER_SIZE, "#0P%u#1P%u#2P%u#3P%uT%u\r\n", as->azimuth, as->shoulder, as->elbow, as->wrist); Serial.print(msg);
		
	
	return flag;
	
}


void commandMode(uint16_t baudrate);

/**********************************************************************
							User Interface
***********************************************************************/

void commandMode(uint16_t baudrate){
	
	Serial1.begin(baudrate);
	
	snprintf(msg, BUFFER_SIZE, "USER CONTROL MODE\r\n"); Serial.print(msg);
		
	
	Serial.println("Type and send one of the following numbers to issue a command.\n");
	Serial.println("0 - Home Position");
	Serial.println("1 - Ready Position");
	Serial.println("2 - Enter Position");
	Serial.println("3 - Enter Position( With calibration) ");
	Serial.println("4 - Write to Servo");
	Serial.println("5 - Open Claw");
	Serial.println("6 - Close Claw");
	Serial.println("7 - Print Current Configuration    (DOES NOT REFLECT WRITES TO INDIVIDUAL MOTORS)");
	Serial.println("8 - Print Current Position         (DOES NOT REFLECT WRITES TO INDIVIDUAL MOTORS)");
	
	int cmd;

	while(1){
		
		cmd = getCommand();

		switch(cmd){
			case 0:
				toFetal();break;
			case 1;
				toReady();break;
			case 2:
				getPosition();break;
			case 3:
				Serial.println("Enter command in the format specified by LynxMotion documentation.");
				Serial.println("EXAMPLE:\t\t#1P1500T1000");
				sendCommand();break;
			case 4:
				openClaw();break;
			case 5:
				close();break;
			case 6:
				printConfig();break;
			case 7:
				printPosition();break;
			default:
				Serial.println("Please enter a Valid Command");
		}
	

	}
}

/*
	Info	: Prompt User to key in three cartersian coordinates
	Param	: Spatial Cartersian Coordinates
	Return	: none
*/
void getPosition( point_t *pt ){

	point_t local = {};
	
	Serial.println("Enter coordinates as prompted:");
	
	Serial.print("x-coordinate:        ");
	local.x = getfloat();
	Serial.println(local.x);
	
	Serial.print("y-coordinate:        ");
	local.y = getfloat();
	Serial.println(local.y);
	
	Serial.print("z-coordinate:        ");
	local.z = getfloat();
	Serial.println(local.z);
	Serial.println("");
	
	pt = &local;
	
}

void toPoint ( point_t *pt ){
	joint_t js = {};
	as_t as = {};
	
	if( !arm_checkBounds(pt) ){
		inverseKinematic(pt, &js);
		as_conversion( &js, &as );
		setConfiguration(&as);
		
	}else{
		// Unreachable
		Serial.println("Coordinates Unreachable.");
	}
}


/************************************************************
					Utility Function
*************************************************************/
/* 	
	Function: floating point constrain check		
	Param	: input value, min and max value
	Return	: constrained output
*/
float fconstrain( float input, float min, float max){
	if( input < min){
		return min;
	}else if (input > max){
		return max;
	}
	return input;
}

/*
	Info	: Reading data from stream
	Return	: signed integer from stream
*/
int16_t getinterger(void){
	// Hold on to the Serial Port, pending incoming data

	char cmd[8] = {};
	uint8_t index = 0;
	
	while( !Serial.available() ){}
	delay(3);

    while( Serial.available() ){

	    cmd[index++] = Serial.read();	// Read each character in buffer
        delay(3);
    }

	return atoi(command);
}

/*
	Info	: Reading data from stream
	Return	: float from stream
*/
float getfloat(void){
	// Hold on to the Serial Port, pending incoming data

	char cmd[8] = {};
	uint8_t index = 0;
	
	while( !Serial.available() ){}
	delay(3);

    while( Serial.available() ){

	    cmd[index++] = Serial.read();	// Read each character in buffer
        delay(3);
    }

	return atof(command);
}


}