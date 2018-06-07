#include "ballCoordinate.h"
#include <stdlib.h>
#include <stdio.h>
#include <arduino.h>

char dataStream[DATALENGTH];
char ballmatrix[BALLPERFRAME];

Ball *balls;
int numOfPoint;


void printStringToken(char** strArr, int n){
  int i = 0;
  for(i = 0;i < n; i++){
    Serial.println(strArr[i]);
  }
}
int getDataStream(void){

  //first clear all data in the struct
  //TODO:

    memset(dataStream, 0, DATALENGTH);
    memset(balls,0,sizeof(balls));

  // Serial Communication with Raspberry Pi on Serial port 1 begins
  Serial.begin(115200);

  // Does nothing until serial transmission begins
  while(Serial.available() == 0 ){Serial.println("input Stream");}

int dataindex = 0;
  while(Serial.available()> 0){

  // Fills datastream
    if(dataindex < DATALENGTH-1)
      {

          char inChar = Serial.read(); // Read a character
		      Serial.print("Parsing:");Serial.println(inChar);
          dataStream[dataindex] = inChar; // Store it
          dataindex++; // Increment where to write next

      }


    delayMicroseconds(100);
  }
  dataStream[dataindex] = '\0'; // Null terminate the string
  Serial.flush();

Serial.print("DataStream(contain):");Serial.println(dataStream);

  // validate package.
  // only use this validation when using hardcoding method.
  // int sizeOfData = sizeof(dataStream)/ sizeof(char);
  // if(sizeOfData ==4){
  //   return 1;
  // }else{
  //   return -1;
  // }

}

bool parseData(void){
  /**
   * This function parse data package
   */
   char **stringToken = NULL;
   stringToken = split(dataStream,';',&numOfPoint);
   printStringToken(stringToken, numOfPoint+1);

   numOfPoint = atoi(stringToken[0]);
   if(numOfPoint == 1){

	   //store the first data point
	   int n = 0;// dummies.
	   char **pointToken = split(stringToken[1],',',&n);

	  // balls[0].color = (char)pointToken1[0][0];
     // balls[0].y = atoi(pointToken[1]);
     // balls[0].z = atoi(pointToken[2]);
     balls[0].color = (uint8_t)'e';
     balls[0].y = (int16_t) 90;
     balls[0].z = (int16_t) 26;

    Serial.println("=====");
    Serial.print("pointOtk:(after)");Serial.println(balls[0].color);
    Serial.print("pointy:(atof)");Serial.println(atoi(pointToken[1]));
     Serial.print("pointy:()");Serial.println(balls[0].y);
     Serial.print("pointz:()");Serial.println(balls[0].z);
  }
   else if(numOfPoint == 2){

	   int n = 0;// dummies.
	   //store the first data point
	   char **pointToken1 = split(stringToken[1],',',&n);
	   balls[0].color = (char)pointToken1[0][0];
     balls[0].y = atoi(pointToken1[1]);
	   balls[0].z = atoi(pointToken1[2]);

	   //store the second data point.
	    char **pointToken2 = split(stringToken[2],',',&n);
	   balls[1].color = (char)pointToken2[0][0];
     balls[1].y = atoi(pointToken2[1]);
	   balls[1].z = atoi(pointToken2[2]);

   }

   // int token = 1;
   // for(int i = 0;i<numOfPoint;i++){
   //
   //   int n =0;
   //   char **pointToken = split(stringToken[token],',',&n);
   //   token++;
   //   balls[i].color = (char)pointToken[0][0];
   //    balls[i].y = atoi(pointToken[1]);
   //   balls[i].z = atoi(pointToken[2]);
   // }
   //TODO:
   // first token; number of data point.
   // optional first or second.
   // if numOfDat is 0 then ignore.
   Serial.println("===print struct===");
   printCoordinate(balls, numOfPoint);
   Serial.println("===");
}


char ** split(const char *str, char delimiter, int *n) {

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


// char* relativeSwap(char *data){
  // char * copy = deepCopy(data);
  // swap the 1st and 2nd
  // char temp1 = *copy[0];
  // copy[0] = copy[1];
  // copy[1] = temp;

  // swap 3rd and 4th
  // char temp2 = *copy[2];
  // *copy[2] = *copy[3];
  // *copy[3] = temp;

  // return copy;
// }

void printCoordinate(Ball *ballCoordinates, int n){
  for(int i =0; i < n;i++){
    Serial.println("");
    Serial.print(ballCoordinates[i].color);
    Serial.print(",");
    Serial.print(ballCoordinates[i].y);
    Serial.print(",");
    Serial.println(ballCoordinates[i].z);
  }
}

char * deepCopy(const char *s) {
  char *copy = (char *) malloc( (strlen(s) + 1) * sizeof(char) );
  strcpy(copy, s);
  return copy;
}
