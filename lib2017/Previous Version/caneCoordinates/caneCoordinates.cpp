#include "caneCoordinates.h"
#include <stdlib.h>
#include <stdio.h>
#include <arduino.h>
 
char datastream[DATALENGTH];
caneset G, Y;

void receivePackage(void){
 // Function waits for incoming data regarding canes, collects it, and then parses it into usable form.
	
	receiveCoordinates();
	parseData();
}

void receiveCoordinates(void){
  // Function simply awaits data transmission from raspberry pi, and deposits it in 
  //  global variable datastream.  
	
  // First, all pre-existing datapoints and incoming strings are cleared.
  memset(datastream, 0, DATALENGTH);
  memset(G.x, 0, DATALENGTH/15);
  memset(G.y, 0, DATALENGTH/15);
  memset(G.z, 0, DATALENGTH/15);
  memset(Y.x, 0, DATALENGTH/15);
  memset(Y.y, 0, DATALENGTH/15);
  memset(Y.z, 0, DATALENGTH/15);
  G.count=0;
  Y.count=0;
  
  // Serial Communication with Raspberry Pi on Serial port 1 begins
  Serial.begin(115200);

  // Local variables for collecting datastream are declared
  int datalength;
  int dataindex = 0;
  char inChar;

  // Does nothing until serial transmission begins
  while(Serial.available() == 0 ){}
  
  while(Serial.available()> 0){
    
	// Fills datastream
    if(dataindex < DATALENGTH-1) 
      {
          inChar = Serial.read(); // Read a character
          datastream[dataindex] = inChar; // Store it
          dataindex++; // Increment where to write next
          datastream[dataindex] = '\0'; // Null terminate the string
      }
    delayMicroseconds(100);
  }
  
  Serial.println(datastream);
}

void parseData(void){
 // This function sorts through datastream to determine the total number of green and 
 //  yellow canes, and stores the count, along with the x,y,z coordinates into the 
 //  arrays of global structures G and Y, respectfully.
 
 int i;
 int xStart, xEnd, yStart, yEnd, zStart, zEnd;
 
 // Iterates through each element of datastream
 for(i=0; i<DATALENGTH; i++){
    
	// If the element is a G
    if(datastream[i] == 71){
      
      xStart = i+2;                                 // Starting index of x coordinate is established
      xEnd = getBound(xStart);                      // getBound retrieves coordinate ending index
      G.x[G.count] = pullCoordinate(xStart, xEnd);  // value of coordinate is pulled from datastream and stored in x array
		
      //Serial.println(G.x[G.count]);
      
      yStart = xEnd+2;							    // Process is repeated for y coordinate
      yEnd = getBound(yStart);
      G.y[G.count] = pullCoordinate(yStart, yEnd);

      //Serial.println(G.y[G.count]);
       
      zStart = yEnd+2;                              // And again for z coordinate
      zEnd = getBound(zStart);
      G.z[G.count] = pullCoordinate(zStart, zEnd);

      //Serial.println(G.z[G.count]);
      G.count+=1;                                   // Green cane count is incremented
    }
    
	// If the element is a Y
    else if(datastream[i] == 89){
      xStart = i+2;
      xEnd = getBound(xStart);
      Y.x[Y.count] = pullCoordinate(xStart, xEnd);
	  
      //Serial.println(Y.x[Y.count]);
      
      yStart = xEnd+2;
      yEnd = getBound(yStart);
      Y.y[Y.count] = pullCoordinate(yStart, yEnd);

      //Serial.println(Y.y[Y.count]);
       
      zStart = yEnd+2;
      zEnd = getBound(zStart);
      Y.z[Y.count] = pullCoordinate(zStart, zEnd);

      //Serial.println(Y.z[Y.count]);
      Y.count+=1;
      
    }
 }

}

int getBound(int startBound){
   // Function determines the next non-numerical character, and returns the index of the 
   //  end bound of the numerical value.
   
   while(datastream[startBound] > 44 && datastream[startBound] < 58){
    startBound++;
   }

  return startBound-1;
}

float pullCoordinate(int Start, int End){
   // Given the start and end bounds of the coordinate from the datastream, this value is placed
   //  in a separate character array, and returned as a floating point number.

   int numlength = End-Start+1;
   char value[numlength];

   for(int j=0; j<numlength; j++){
    value[j] = datastream[Start+j];
   }

   return atof(value);
}

void addDist(void){
	
	int i;
	for(i=0; i<G.count; i++){
		float x = G.x[i];
		float y = G.y[i];
		G.dist[i] = sqrt(x*x+y*y);
		//Serial.println(G.dist[i]);
	}
	
	for(i=0; i<Y.count; i++){
		float x = Y.x[i];
		float y = Y.y[i];
		Y.dist[i] = sqrt(x*x+y*y);
		//Serial.println(Y.dist[i]);
	}
}

void sortCanesets(void){
	
  int i;
  float insertDist;
  int insertIndex;
  
  for(i = 1; i<G.count; i++)
  {
    insertIndex = i;
    insertDist = G.dist[i];
	float insertX = G.x[i];
	float insertY = G.y[i];
	float insertZ = G.z[i];
	G.Status[i] = 0;
	G.xrob[i] = 0;

    while(insertIndex > 0 && G.dist[insertIndex-1] > insertDist)
    {
        G.dist[insertIndex] = G.dist[insertIndex - 1];
		G.x[insertIndex] = G.x[insertIndex - 1];
		G.y[insertIndex] = G.y[insertIndex - 1];
		G.z[insertIndex] = G.z[insertIndex - 1];
        insertIndex--;
    }
    
    if(insertIndex != i)
    {
        G.dist[insertIndex] = insertDist;
		G.x[insertIndex] = insertX;
		G.y[insertIndex] = insertY;
		G.z[insertIndex] = insertZ; 
    }   
     
  }
  
  for(i = 1; i<Y.count; i++)
  {
    insertIndex = i;
    insertDist = Y.dist[i];
	float insertX = Y.x[i];
	float insertY = Y.y[i];
	float insertZ = Y.z[i];
	Y.Status[i] = 0;

    while(insertIndex > 0 && Y.dist[insertIndex-1] > insertDist)
    {
        Y.dist[insertIndex] = Y.dist[insertIndex - 1];
		Y.x[insertIndex] = Y.x[insertIndex - 1];
		Y.y[insertIndex] = Y.y[insertIndex - 1];
		Y.z[insertIndex] = Y.z[insertIndex - 1];
        insertIndex--;
    }
    
    if(insertIndex != i)
    {
        Y.dist[insertIndex] = insertDist;
		Y.x[insertIndex] = insertX;
		Y.y[insertIndex] = insertY;
		Y.z[insertIndex] = insertZ; 
    }   
     
  }
  
  Serial.print("{");
  for(i = 0; i< G.count; i++)
  {
    float val = G.dist[i];
    Serial.print(val);
    if(i!= G.count-1){Serial.print(", ");}
  }
  Serial.print("}\n");
  
  Serial.print("{");
  for(i = 0; i< Y.count; i++)
  {
    float val = Y.dist[i];
    Serial.print(val);
    if(i!= Y.count-1){Serial.print(", ");}
  }
  Serial.println("}");
	
}
