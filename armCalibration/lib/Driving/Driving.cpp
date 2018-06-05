/*
Author:	CheeTown Liew
Version of June 10, 2017

This algorithm is designed to drive the robot and run with precise displacement
using motor encoder, the algorithm includes simple proportion controller and
displacement-encoder count convertor, speed comparator and path tracking.

Micro-controller Board: Arduino Mega
Motor Driver: Sabertooth Motor Driver

*/

#include <Driving.h>
#include <math.h>

SabertoothSimplified motordriver(Serial3);

//declare global & local coordinates
float global_x = 0.0;
float global_y = 0.0;
float local_x = 0.0;
float local_y = 0.0;
int16_t global_orientation = 0;

//declare encoder counter
volatile int64_t ECL = 0;    //encoder counter - LEFT
volatile int64_t ECLM = -1;  //encoder counter - LEFT MEMORY
volatile int64_t ECR = 0;    //encoder counter - RIGHT
volatile int64_t ECRM = -1;  //encoder counter - RIGHT MEMORY

//declare variable
bool debug1 = false;

void debug ( bool toggle ){
	debug1 = toggle;
}

//Default initiattion to assign I/O pins
void dinit (){
	//For Channel A
	attachInterrupt(digitalPinToInterrupt(ECLA), counter1, RISING); //LEFT encoder channel A trigger
	attachInterrupt(digitalPinToInterrupt(ECRA), counter2, RISING); //RIGHT encoder channel A trigger

	//For Channel B
	pinMode(ECLB, INPUT);   //LEFT encoder channel B as input
	digitalWrite(ECLB, LOW);  //Pull down LEFT channel B
	pinMode(ECRB, INPUT);   //RIGHT encoder channel B as input
	digitalWrite(ECRB, LOW);  //Pull down RIGHT channel B

	//Encoder's Vcc
	pinMode(24, OUTPUT);
	pinMode(25, OUTPUT);
	pinMode(26, OUTPUT);
	pinMode(27, OUTPUT);
	digitalWrite(24, HIGH);
	digitalWrite(25, HIGH);
	digitalWrite(26, LOW);
	digitalWrite(27, LOW);

	Serial3.begin(19200);
}

//Initiator with customize I/O pins, the argument must be pointer to an array with
//the format of { Left_Channel_A, Right_Channel_A, Left_Channel_B, Right_Channel_B };
/* void dinit ( uint8_t& pins ){
	attachInterrupt( digitalPinToInterrupt( *pins ), counter1, RISING);
	attachInterrupt( digitalPinToInterrupt( *(pins+1) ), counter2, RISING);

	pinMode( *(pins+2), INPUT);   //LEFT encoder channel B as input
	digitalWrite(*(pins+2), LOW);  //Pull down LEFT channel B
	pinMode( *(pins+3), INPUT);   //RIGHT encoder channel B as input
	digitalWrite( *(pins+4), LOW);  //Pull down RIGHT channel B

	//Encoder's Vcc
	pinMode(EPOWERL, OUTPUT);
	digitalWrite(EPOWERL, HIGH);
	pinMode(EPOWERR, OUTPUT);
	digitalWrite(EPOWERR, HIGH);
} */



//Left Encoder Counter
void static counter1() {
	ECLM = ECL;
	if (digitalRead(ECLB) == HIGH) {ECL++;}
	else {ECL--;}
}

//Right Encoder Counter
void static counter2() {
	ECRM = ECR;
	if (digitalRead(ECRB) == HIGH) {ECR++;}
	else {ECR--;}
}

int64_t D2C ( float distance ) {
	return ((double) distance * (double) INCH2C) ;
}

float C2D (int64_t EncoderCount ){
	return ((double) EncoderCount * (double) COUNTER2INCHE);
}

int comparator( int64_t L, int64_t R ){
	return ( ECL - L ) - ( ECR - R );
}

int16_t leftDrive = 0;
int16_t driving2 = 0;
int16_t na = 0;

int64_t R2C ( float angle ){
	return D2C ( (  angle / 360.0 * WHEELDIA * 3.141592653589 ) );
}

int16_t C2R ( int64_t encoderCount ){
	return C2D ( encoderCount ) / ( WHEELDIA * 3.141592653589 ) * 360;
}

void steer(float angle){
	const int64_t left_targetcount = ECL + R2C(toAngle + 2);
	const int64_t right_targetcount = ECR - R2C(toAngle + 2);


	int ECLO = -1;
	int ECRO = -1;

	// Encoder count left/right increment
	const int ECLI = ECL;
	const int ECRI = ECR;

	while ( true ) {
		leftDrive = constrain( (double) K1 * (left_targetcount - ECL), -113, 112);
		driving2 = constrain( (double) K2 * (right_targetcount - ECR), -127, 127);

		/*
		the comparison value of the encoder count difference per iteration
		positive mean left motor is faster than the right motor and vise versa
		*/

		int16_t v = comparator( ECL, ECR );

		if( (v*(float) V) > 30.0 ){} //Serial.println("Warning! There is a large speed differential.");
		else v *= (float) V;

		// adjust the increment of each wheel if one go faster than the other.
		if(v > 0){// left counter is greater than R

		}
}



void steer ( int16_t toAngle ){
	const int64_t left_targetcount = ECL + R2C(toAngle + 2);
	const int64_t right_targetcount = ECR - R2C(toAngle + 2);
	/*
	Serial.println((int32_t)R2C(toAngle));
	Serial.println((int32_t)left_targetcount);
	Serial.println((int32_t)right_targetcount);
	*/
	leftDrive = 0;
	driving2 = 0;
	na = 0;
	int8_t pause = 0;
	uint64_t timestamp = 0;
	int ECLO = -1;
	int ECRO = -1;
	const int ECLI = ECL;
	const int ECRI = ECR;

	while ( true ) {
		leftDrive = constrain( (double) K1 * (left_targetcount - ECL), -113, 112);
		driving2 = constrain( (double) K2 * (right_targetcount - ECR), -127, 127);

/*
the comparison value of the encoder count difference per iteration
positive mean left motor is faster than the right motor and vise versa
*/

		int16_t v = comparator( ECLO, ECRO );
		if( (v*(float) V) > 30.0 ){} //Serial.println("Warning! There is a large speed differential.");
		else v *= (float) V;
		if(toAngle > 0 ){

			if( v > 0 ){
				if (leftDrive > 0){
					leftDrive -= v* (float) V;
				}else{
					leftDrive += v* (float) V;
				}
			} else {
				if (driving2 > 0){
					driving2 -= v* (float) V;
				}else{
					leftDrive += v* (float) V;
				}
			}
		}else {
			if( v > 0 ){
				if (leftDrive > 0){
					leftDrive -= v* (float) V;
				}else{
					leftDrive += v* (float) V;
				}
			} else {
				if (driving2 > 0){
					driving2 -= v* (float) V;
				}else{
					leftDrive += v* (float) V;
				}
			}

		}

		int16_t integral = comparator( ECLI, ECRI );
		// if( integral*  (float) I > 20 ) Serial.println("Warning! There is a large integral difference");
		// if(toAngle > 0 ){

			// if( integral > 0 ){
				// if (leftDrive > 0){
					// leftDrive -= integral* (float) I;
				// }else{
					// driving2 += integral* (float) I;
				// }
			// } else {
				// if (driving2 > 0){
					// driving2 -= integral* (float) I;
				// }else{
					// leftDrive += integral* (float) I;
				// }
			// }
		// }else {
			// if( integral > 0 ){
				// if (leftDrive < 0){
					// leftDrive += integral* (float) I;
				// }else{
					// driving2 += integral* (float) I;
				// }
			// } else {
				// if (driving2 < 0){
					// driving2 -= integral* (float) I;
				// }else{
					// leftDrive -= integral* (float) I;
				// }
			// }

		// }

		if (toAngle> 0){
			leftDrive = constrain( leftDrive, 0, 112);
			driving2 = constrain( driving2, -127, 0);
		}else{
			leftDrive = constrain(leftDrive, -113, 0);
			driving2 = constrain(driving2, 0, 127);
		}




		motordriver.motor(1, leftDrive);
		motordriver.motor(2, driving2);

		//if the motor(s) stall for 0.1s, quit the iteration
		if ( ECL == ECLO || ECR == ECRO ) {
			if ( pause == 0 ) {
				timestamp = millis();
				pause = 1;
			} else {
				if ( abs(millis() - timestamp) > 100 ) {
					pause = 0;
					break;
				}
			}
		}

		ECLO = ECL;
		ECRO = ECR;
		//if (debug == true)debugMode();
		/*
		Serial.print( (int32_t) ECL);
		Serial.print("\t");
		Serial.print( (int32_t) ECR);
		Serial.print("\t");
		Serial.print(v);
		Serial.print("\t");
		Serial.print(integral);
		Serial.print("\t");
		Serial.print(leftDrive);
		Serial.print("\t");
		Serial.print(driving2);
		Serial.print("\n");
		*/
	}

	motordriver.motor(1, 0);
	motordriver.motor(2, 0);


	global_orientation +=  ( ( C2R ( (int64_t) ECL - (int64_t) ECLI ) -  C2R( ( int64_t) ECR - (int64_t) ECRI ))/2 );
	//global_orientation += toAngle;
	//global_orientation = constrain(global_orientation, -180, 180);

	//Serial.println(global_orientation);

}

void driveto( float distance ) {
	const int64_t left_targetcount = ECL+ D2C( distance + 0.15 );
	const int64_t right_targetcount = ECR + D2C( distance + 0.15 );
	leftDrive = 0;
	driving2 = 0;
	na = 0;
	int8_t pause = 0;
	uint64_t timestamp = 0;
	int ECLO = -1;
	int ECRO = -1;
	const int ECLI = ECL;
	const int ECRI = ECR;

	while ( true ) {
		leftDrive = constrain( (double) K1 * (left_targetcount - ECL), -113, 112);
		driving2 = constrain( (double) K2 * (right_targetcount - ECR), -127, 127);

/*
the comparison value of the encoder count difference per iteration
positive value indicates left motor is faster than the right motor and vise versa
*/
		int16_t v = comparator( ECLO, ECRO );
		if( (v*(float) V) > 30.0 ){} //Serial.println("Warning! There is a large speed differential.");
		else v *= (float) V;
		if(distance > 0 ){

			if( v > 0 ){
				if (leftDrive > 0){
					leftDrive -= v* (float) V;
				}else{
					leftDrive += v* (float) V;
				}
			} else {
				if (driving2 > 0){
					driving2 -= v* (float) V;
				}else{
					leftDrive += v* (float) V;
				}
			}
		}else {
			if( v > 0 ){
				if (leftDrive > 0){
					leftDrive -= v* (float) V;
				}else{
					leftDrive += v* (float) V;
				}
			} else {
				if (driving2 > 0){
					driving2 -= v* (float) V;
				}else{
					leftDrive += v* (float) V;
				}
			}

		}

		int16_t integral = comparator( ECLI, ECRI );

		if( integral*  (float) I > 20 ){}  //Serial.println("Warning! There is a large integral differences.");
		if(distance > 0 ){

			if( integral > 0 ){
				if (leftDrive > 0){
					leftDrive -= integral* (float) I;
				}else{
					driving2 += integral* (float) I;
				}
			} else {
				if (driving2 > 0){
					driving2 -= integral* (float) I;
				}else{
					leftDrive += integral* (float) I;
				}
			}
		}else {
			if( integral > 0 ){
				if (leftDrive < 0){
					leftDrive -= integral* (float) I;
				}else{
					driving2 += integral* (float) I;
				}
			} else {
				if (driving2 < 0){
					driving2 += integral* (float) I;
				}else{
					leftDrive -= integral* (float) I;
				}
			}

		}


		if (distance > 0){
			//forward motion
			leftDrive = constrain( leftDrive, 0, 112);
			driving2 = constrain( driving2, 0, 127);
		}else{
			//backward motion
			leftDrive = constrain(leftDrive, -113, 0);
			driving2 = constrain(driving2, -127, 0);
		}




		motordriver.motor(1, leftDrive);
		motordriver.motor(2, driving2);

		//if the motor(s) stall for 0.1s, quit the iteration
		if ( ECL == ECLO || ECR == ECRO ) {
			if ( pause == 0 ) {
				timestamp = millis();
				pause = 1;
			} else {
				if ( abs(millis() - timestamp) > 100 ) {
					pause = 0;
					break;
				}
			}
		}

		ECLO = ECL;
		ECRO = ECR;
		//if (debug == true)debugMode();
		/*
		Serial.print( (int32_t) ECL);
		Serial.print("\t");
		Serial.print( (int32_t) ECR);
		Serial.print("\t");
		Serial.print(v);
		Serial.print("\t");
		Serial.print(integral);
		Serial.print("\t");
		Serial.print(leftDrive);
		Serial.print("\t");
		Serial.print(driving2);
		Serial.print("\n");
		*/
	}

	motordriver.motor(1, 0);
	motordriver.motor(2, 0);

	global_x += ( C2D( ECL - ECLI) + C2D (ECR - ECRI) )/2 * cos(global_orientation);
	global_y += ( C2D( ECL - ECLI) + C2D (ECR - ECRI) )/2 * sin(global_orientation);
	//global_x = C2D (( ECL + ECR )/2) ;

}

void debugMode (){
	Serial.begin(9600);
	Serial.print(na);
	Serial.print("\t");
	Serial.print("ECL\t");
	Serial.print( (int32_t) ECL);
	Serial.print("\tECR\t");
	Serial.print( (int32_t) ECR);
	Serial.print("\tV1\t");
	Serial.print(leftDrive);
	Serial.print("\tV2\t");
	Serial.println(driving2);
}
