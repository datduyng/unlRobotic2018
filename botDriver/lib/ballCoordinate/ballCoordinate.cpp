#include "ballCoordinate.h"
#include <stdlib.h>
#include <stdio.h>
#include <arduino.h>

char dataStream[DATALENGTH];
uint8_t numOfPoint;

uint8_t* color;
int16_t* y;
int16_t* z;

int getDataStream(void){
  // code Reuse fro last year competition

  //first clear all data in the struct
  memset(dataStream, 0, DATALENGTH);


  // Serial Communication with Raspberry Pi on Serial port 1 begins
  Serial.begin(115200);

  // Does nothing until serial transmission begins
  while(Serial.available() == 0 ){}

  int dataindex = 0;
  while(Serial.available()> 0){

  // Fills datastream
    if(dataindex < DATALENGTH-1)
      {
          char inChar = Serial.read(); // Read a character
          dataStream[dataindex] = inChar; // Store it
          dataindex++; // Increment where to write next
      }
    delayMicroseconds(100);
  }
  dataStream[dataindex] = '\0'; // Null terminate the string
  Serial.flush();

  Serial.print("DataStream(contain): ");Serial.println(dataStream);
}

bool parseData(void){

  //split package  to main token by ';'
  char **stringToken = NULL;
  stringToken = split(dataStream,';',&numOfPoint);

  //store number of coordinate is passing
  numOfPoint = atoi(stringToken[0]);

  // exit if receive bad data;
  //TODO: added more validation
  if(numOfPoint > 2 || numOfPoint < 1) {
    //sSerial.println("fail in parse");
    return false;
  }

  //set dynamic array size.
  //create dynamic array size.
  // make sure it only have a size of 1 or 2 acordingly to numOfPoint.
  color = (uint8_t*) malloc(sizeof(uint8_t) * numOfPoint);
  y = (int16_t*) malloc(sizeof(int16_t) * numOfPoint);
  z = (int16_t*) malloc(sizeof(int16_t) * numOfPoint);

  char **pointToken = NULL;
  for(int i = 0;i < numOfPoint; i++){
    uint8_t n = 0 ;// dummies varable for debug or output valiedation
    pointToken = split(stringToken[i+1],',',&n);

    //deposit in to global variable
    color[i] = atoi(pointToken[0]);
    y[i] = atoi(pointToken[1]);
    z[i] = atoi(pointToken[2]);
  }


  // free all 2d array token to prevent mem leak
  for(int i = 0; i < numOfPoint; i++) free(pointToken[i]);
  free(pointToken);
  for(int i = 0; i < numOfPoint; i++) free(stringToken[i]);
  free(stringToken);


  //TODO: ERROR checking and return true if success ful else return false;
  printPoint();
  return true;
}

void printPoint(){
  for(int i = 0; i<numOfPoint;i++){
    Serial.print(color[i]);Serial.print(",");Serial.print(y[i]);Serial.print(",");Serial.print(z[i]);
    Serial.println("");
  }
}


char ** split(const char *str, char delimiter, uint8_t *n) {

  int i, numDelimiters = 0;
  char delimit[] = {delimiter, '\0'};
  char *s = deepCopy(str);
  for(i=0; i<strlen(str); i++) {
    if(str[i] == delimiter) {
      numDelimiters++;
    }
  }

  char **result = (char **) malloc(sizeof(char**) * (numDelimiters+1));
  char *token = strtok(s, delimit);
  i = 0;
  while(token != NULL) {
    result[i] = deepCopy(token);
    token = strtok(NULL, delimit);
    i++;
  }

  free(s);
  *n = (numDelimiters+1);
  return result;
}

char * deepCopy(const char *s) {
  char *copy = (char *) malloc( (strlen(s) + 1) * sizeof(char) );
  strcpy(copy, s);
  return copy;
}
