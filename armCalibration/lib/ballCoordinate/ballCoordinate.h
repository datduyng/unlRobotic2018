/**
 * @Author Dat nguyen
 *
 * This library establish a protocol for transmitting
 * data from the raspberry pi to arduino
 * The data need to be format in the following manner
 *
 * RBNG
 * R: Red
 * B; blue
 * G: green
 * N: NUll;
 * By assuming that x,y,z axis of the ball is
 * a fixed position on every frame,
 * we can use method of 1,0 2 by matrix.
 * to collect field configueation
 *
 * this lib will asume that ball matrix is indexed like as following
 *
 *        |1|2|
 *        |3|4|
 */

 #ifndef BALL_COORDINATES   /* Include guard */
 #define BALL_COORDINATES

#include "stdint.h"
#include "stdlib.h"

#define DATALENGTH 4
#define BALLPERFRAME 4

extern char dataStream[DATALENGTH];

extern char ballMatrix[BALLPERFRAME];

// FUNCTION

/**
 * This function get data transmited from raspberry pi then store
 * data in dataStream char array  variable(gloabl variable)
 * return 1 if valid dataStream length got recieve
 */
int getData(void);

/**
 * This function parse data and store it global struct array
 * store in balls( Struct array)
 * if a position does not have a ball then it that position of the array
 * will point to null
 */

/**
 * This function perform column transformation
 * this function will swap the 1st and 2nd .
 * swap 3rd and 4th eleent of the matrix
 */
char* relativeSwap(char *matrix);

/*
 * This function deepcopy a array
 */
char * deepCopy(const char *s);

#endif
