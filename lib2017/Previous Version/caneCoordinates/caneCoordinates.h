// TITLE : caneCoordinates
// AUTHOR: Lukas Renker

// PURPOSE: The caneCoordinates library includes the necessary functions and structures to
// accept data regarding canes, their color, and their position from the Raspberry Pi software
// executing the image processing. 
//
// First, this library establishes a protocol for transmitting the data. The data must adhere to
// the following format:
//              
//                       CANE 1                     CANE 2
//          |---------------------------||---------------------------|
//          [G/Y][x][n.n][y][n.n][z][n.n][G/Y][x][n.n][y][n.n][z][n.n][G/Y][x]... etc.
//
//
// Each cane begins with a capitol G, or Y, depending on its color. Then, x, followed by an arbitrarily
// sized coordinate of the position of the cane relative to the cameras, in the x direction. The same is 
// done for the y and z directions, as can be seen below in the example string.
//
//  EX:     Gx5.567y6.7z4.58Yx5.45y6.7z4.58Gx2.45y1.7z4.0Yx1.23y4.7z8Gx1.23y4.56z1.4 
//
//  The function receiveCoordinates() awaits an incoming string of data, collects it, and then parseData()
//  sorts the string into the two global structures, G and Y, for later use.

#ifndef CANE_COORDINATES   /* Include guard */
#define CANE_COORDINATES

#include "stdint.h"

#define DATALENGTH 150

extern char datastream[DATALENGTH];
typedef struct{
  int count;
  float x[DATALENGTH/15];
  float y[DATALENGTH/15];
  float z[DATALENGTH/15];
  float dist[DATALENGTH/15];
  float xrob[3];
  float x_init[8];
  uint8_t Status[DATALENGTH/15];
}caneset;

extern caneset G, Y;

// FUNCTIONS ---------------------------------------------------------------------//
void receivePackage(void);
void receiveCoordinates(void);
void parseData(void);
int getBound(int startBound);
float pullCoordinate(int Start, int End);
void sortCanesets(void);
void addDist(void);

#endif