
/**
 * @Author Dat Nguyen 
 * Version Date: 7/26/18 
 * 
 * This program run pro mini to retrieve ultrasonic sensor
 * this program also establish protocol with arduino mega 
 * - the main bot.
 */
const byte address = 8;
char buffer[100];

void receiveEvent(int howMany) {
  received = howMany;
  memset(buffer, 0, maxlength);

  for (int i = 0; i < howMany; i++) {
    buffer[i] = Wire.read();
  }
  if(strcmp(buffer,"sonar") == 0){
    //TODO: take measurement and retrieve data from all 
    // ultrasonic. 
    
  }
}

void updateSonar(){
  
}
void setup() {
  // put your setup code here, to run once:
  Wire.begin(address);                // join i2c bus with address #8

}

void loop() {
  // put your main code here, to run repeatedly:
  
}
