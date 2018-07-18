#include <SabertoothSimplified.h>
#include <RaisingCamera.h>


SabertoothSimplified cameradrive(Serial2);


const double COUNTERPERREVOLUTION = 227.1 * 48/ 4;

volatile long EC3 = 0;
volatile long EC3M = -1;

static void counter3 ( void ) {
	EC3M = EC3;
	if (digitalRead(22) == HIGH) {
	EC3++;
	}  else {
	EC3--;
	}
}

void cinit(){
	Serial.begin(115200);
	Serial2.begin(19200);

	attachInterrupt(digitalPinToInterrupt(21), counter3, RISING);
	pinMode(23, OUTPUT);
	digitalWrite(23, LOW);
	pinMode(22, INPUT_PULLUP);
	
	
}

void RGB(void){
	raising( A2C (130));
	raising( A2C (70));
	Serial.println("Do");
	raising( A2C (-200));
}



int A2C ( int angleofrotation ){
	return (float) angleofrotation / 360 *COUNTERPERREVOLUTION;
}

void raising ( long increment ) {
	long target = increment + EC3;
	long actuation;
	int8_t pause = 0;
	long EC3O = EC3 - 10;
	long timestamp = 0;

	while ( true) {
	actuation = (float) 0.7* (target - EC3);
	actuation  = constrain ( actuation, -64, 64);
	cameradrive.motor(2, actuation);

	if ( EC3O == EC3  ) {
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

	EC3O = EC3;
	delay(10);
	// Serial.print(actuation);
	// Serial.print("\t");
	// Serial.print(EC3);
	// Serial.print("\t");
	// Serial.print(pause);
	// Serial.print("\n");

	}

	cameradrive.motor(2, 0);

}

void raising2 ( long increment ) {
  long target = increment + EC3;
  long actuation;
  int8_t pause = 0;
  long EC3O = EC3 - 10;
  long timestamp = 0;

  while ( true) {
    actuation = (float) 0.7* (target - EC3);
    actuation  = constrain ( actuation, -10, 10);
    cameradrive.motor(2, actuation);

    if ( EC3O == EC3  ) {
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
    
    EC3O = EC3;
    delay(10);
    // Serial.print(actuation);
    // Serial.print("\t");
    // Serial.print(EC3);
    // Serial.print("\t");
    // Serial.print(pause);
    // Serial.print("\n");
    
  }
  
  cameradrive.motor(2, 0);

}

void upRGB (void){
	raising( A2C (140));
	raising( A2C (62));
}

void downRGB (void){
	raising( A2C(-210));
}

