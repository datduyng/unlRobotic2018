#include "caneCoordinate.h"
#include <stdlib.h>
#include <stdio.h>
#include <arduino.h>

char dataStream[DATALENGTH];
char ballmatrix[BALLPERFRAME];


int getData(void){

  //first clear all data in the struct
  memset(ballMatrix,0,BALLPERFRAME);

  // Serial Communication with Raspberry Pi on Serial port 1 begins
  Serial.begin(115200);

  // Does nothing until serial transmission begins
  while(Serial.available() == 0 ){}

  while(Serial.available()> 0){

  // Fills datastream
    if(dataindex < DATALENGTH-1)
      {
          inChar = Serial.read(); // Read a character
          dataStream[dataindex] = inChar; // Store it
          dataindex++; // Increment where to write next
          dataStream[dataindex] = '\0'; // Null terminate the string
      }
    delayMicroseconds(100);
  }

  Serial.println(datastream);

  // validate package.
  int sizeOfData = sizeof(dataStream)/ sizeof(char);
  if(sizeOfData ==4){
    return 1;
  }else{
    return -1;
  }

}

char* relativeSwap(char *data){
  char * copy = deepCopy(data);
  // swap the 1st and 2nd
  char temp1 = *copy[0];
  *copy[0] = *copy[1];
  *copy[1] = temp;

  // swap 3rd and 4th
  char temp2 = *copy[2];
  *copy[2] = *copy[3];
  *copy[3] = temp;

  return copy;
}

char * deepCopy(const char *s) {
  char *copy = (char *) malloc( (strlen(s) + 1) * sizeof(char) );
  strcpy(copy, s);
  return copy;
}
