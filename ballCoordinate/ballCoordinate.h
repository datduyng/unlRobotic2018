/**
 * @Author Dat nguyen
 *
 * This library establish a protocol for transmitting
 * data from the raspberry pi to arduino
 * The data need to be format in the following manner
 * 2;66,12,32;82,43,53;
 * 1;82,103,12;
 *
 * Where:
 * 2(1st Token): # of coordinate that contain in the package
 * 66(2nd Token,1st Point): represent the color of the ball.represent in ascii of the char 'B'
 * 12(2nd Token,2nd Point): y-axis value being pass
 * 32(2nd Token,3rd Point): z-axis value being pass
 * By using ascii number representation of a char, we will ensure getting a valid Character data.
 * Ascii representation:
 * 'B'=66
 * 'b'=98
 * 'R'=82
 * 'r'=114
 * 'G'=71
 * 'g'=103
 *
 * Unit: Raspbery pi will pass pixel value
 */

 #ifndef BALL_COORDINATES   /* Include guard */
 #define BALL_COORDINATES

#include "stdint.h"
#include "stdlib.h"
#include <Arduino.h>

#define DATALENGTH 100

extern char dataStream[DATALENGTH];
extern uint8_t* color;
extern uint8_t* simpleCoordinate;
extern int16_t * y;
extern int16_t* z;
extern uint8_t numOfPoint;


/**Constant**/
// Raspbery pi info
#define SCREEN_WIDTH 720.00
#define SCREEN_LENGTH 1024.00
#define X_CAMERA_TO_ARM 4.47401575
#define Y_CAMERA_TO_ARM 6.4125

#define COOR_X_1 -5.7
#define COOR_Y_1 2.8
#define COOR_Z_1 7.5

#define COOR_X_2 -5.0
#define COOR_Y_2 2.9
#define COOR_Z_2 10.5

#define COOR_X_3 -5.2
#define COOR_Y_3 5.5
#define COOR_Z_3 12.0

#define COOR_X_4 -5.7
#define COOR_Y_4 7.0
#define COOR_Z_4 9.10

#define COOR_X_DROP 0//-5.0
#define COOR_Y_DROP 7.0//9.0
#define COOR_Z_DROP 10.0//10.50


// FUNCTION

/**
 * This function print the point exist in 3 global array (color,y,z)
 * return
 * param
 */
void printPoint();

void printSimplePoint(void);

/**
 * This function get data transmited from raspberry pi then store
 * data in dataStream char array  variable(gloabl variable)
 * return 1 if valid dataStream length got recieve
 */
bool getDataStream(void);

/**
 *This function will parse data by using split function
 * to get it token then put it on to a map/struct
 * return true if data is in correct format. or doesnot spot any wierd data.
 * return false; if spot incorrect formatted data.
 */
bool parseData(void);

/**
 * This method compute and transform the pixel coordinate
 * passed over by the PI to actual coordinate in inches
 * that respect to Robot Arm Base
 *
 * param xVal(pixel) and yVal(pixel) and zVal(value retrieve from
 * ultrasonic sensor
 *
 * return Nothing
 * Change xVal and yVal by reference.
 */
void transformation(double *xVal, double *yVal,const double zVal);

/**
 * This method parse the data using split function
 * using hard coded value then send it to the arm.
 * return true if it parse success full
 */
bool parseSimpleData(void);

/**
 * This method convert the given
/**
 * This function split the input string by a delimiter
 * then return a DEEp copy of a 2d array
 * n is the number of element got splited.( optional);
 */
char ** split(const char *str, char delimiter, uint8_t *n);

/*
 * This function deepcopy a array
 */
char * deepCopy(const char *s);

#endif