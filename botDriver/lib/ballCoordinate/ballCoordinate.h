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

#define DATALENGTH 100
#define BALLPERFRAME 4

extern char dataStream[DATALENGTH];

typedef struct Ball{
	uint8_t color;
	int y;
  int z;
};

extern Ball *balls;
extern int numOfPoint;
// FUNCTION

/**
 * This function get data transmited from raspberry pi then store
 * data in dataStream char array  variable(gloabl variable)
 * return 1 if valid dataStream length got recieve
 */
int getDataStream(void);

/**
 *This function will parse data by using split function
 * to get it token then put it on to a map/struct
 * return true if data is in correct format. or doesnot spot any wierd data.
 * return false; if spot incorrect formatted data.
 */
bool parseData(void);

/**
 * This function split the input string by a delimiter
 * then return a 2d array
 * n is the number of element got splited.( optional);
 */
char ** split(const char *str, char delimiter, int *n);

/**
 * This function perform column transformation
 * this function will swap the 1st and 2nd .
 * swap 3rd and 4th eleent of the matrix
 */
char* relativeSwap(char *matrix);

void printCoordinate(Ball *ballCoordinates, int n);
/*
 * This function deepcopy a array
 */
char * deepCopy(const char *s);

#endif
