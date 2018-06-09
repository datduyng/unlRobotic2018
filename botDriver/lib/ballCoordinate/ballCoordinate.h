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
 */

 #ifndef BALL_COORDINATES   /* Include guard */
 #define BALL_COORDINATES

#include "stdint.h"
#include "stdlib.h"

#define DATALENGTH 100

extern char dataStream[DATALENGTH];
extern uint8_t* color;
extern int16_t * y;
extern int16_t* z;
extern uint8_t numOfPoint;


<<<<<<< HEAD
=======
typedef struct Ball{
	uint8_t color;
	int y;
	int z;
};
>>>>>>> cdb02aa9a0de493e1ac5dcbac69ae34ceaae7b5c

// FUNCTION

/**
 * This function print the point exist in 3 global array (color,y,z)
 * return
 * param
 */
void printPoint();

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
 * then return a DEEp copy of a 2d array
 * n is the number of element got splited.( optional);
 */
char ** split(const char *str, char delimiter, uint8_t *n);

/*
 * This function deepcopy a array
 */
char * deepCopy(const char *s);

#endif
