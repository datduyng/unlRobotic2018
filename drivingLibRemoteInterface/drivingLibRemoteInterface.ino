// include library
#include <IRremote.h>
#include<Driving.h>
#include<math.h>
#include<stdlib.h>


#define IRreceiver 11

IRrecv irrecv(IRreceiver);
decode_results results;
decode_results goToResults;

/*
  * This function will reset the arduino board
  */
void(*resetFunc)(void)=0;
/**
 * This function result robot move in competition sequence
 * @ return none
 */
void sequenceDrive();

char *distInStr;
int i;
void setup() {
   Serial.begin(9600);
   irrecv.enableIRIn();
   dinit();
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
 if(irrecv.decode(&results)){// if receive IR signal    
      switch(results.value){
        case 551489775:// ONOFF
        digitalWrite(13,HIGH);
        break;

        case 551528790:// e
        goTo();
        break;

//        case 551520375:// key1
//        digitalWrite(13,OFF);
//        driveto(10);
//        break;

        case 551504055:// key2
        driveto(10);
        break;

//        case 551536695:// key3
//        steer(90);
//        break;

        case 551495895:// key4
        steer(-90);
        break;

//        case 551528535:// key5
//        sequenceDrive();
//        break;

        case 551512215:// key6
        steer(90);
        break;
//
//         case 551544855:// key7
//        digitalWrite(relay,RELAY_ON);
//        digit = 70;
//        break;
//
        case 551491815:// key8
        driveto(-10);
        break;
//
//        case 551524455:// key9
//        digitalWrite(relay,RELAY_ON);
//        digit =90 ;
//        break;
//        case 551487735:// key0
//        digitalWrite(relay,RELAY_ON);
//        digit = 0;
//        break;
        
        default:
        break;
     }// end switch funciton
 }// end big if
}// end loop 

void goTo(){
  while(true){
    if(irrecv.decode(&goToResults)){// if receive IR signal    
      switch(goToResults.value){
        case 551489775:// ONOFF
        resetFunc();
        break;

        case 551520375:// key1
        addTodistInStr('1');
        break;

        case 551504055:// key2
        addTodistInStr('2');
        break;

        case 551536695:// key3
        addTodistInStr('3');
        break;

        case 551495895:// key4
        addTodistInStr('4');
        break;

        case 551528535:// key5
        addTodistInStr('5');
        break;

        case 551512215:// key6
        addTodistInStr('6');
        break;

         case 551544855:// key7
        addTodistInStr('7');
        break;

        case 551491815:// key8
        driveto(-10);
        break;

        case 551524455:// key9
        addTodistInStr('9');
        break;
        
        case 551487735:// key0
        addTodistInStr('0');
        break;

        //TODO: chane this to actual IR value
        // this is not right
        case 551487735:// +
        addTodistInStr('+');
        break;

        case 551487735:// -
        addTodistInStr('-');
        break;
        
        default:
        break;
     }// end switch funciton
 }// end big if
  }
}

void addTodistInStr(char c){
  distInStr[i] = c;
  i++;
  if(distInStr[0] != '+' || distInStr[0] != '-'){
    i =0;
    distInStr[0] = '\0'; // clear the string
    loop();
  }
  if(i == 3){
    distInStr[i+1]='\0';
    i = 0;// restart 
    driveto(atof(distInStr));
    loop();
  }
}

