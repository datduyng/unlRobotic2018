#include<Driving.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// initialize the interface pins
LiquidCrystal_I2C lcd(0x27, 16, 2); // set address 
const int sonarOffset[4] = {-2,-1, -2,-1};
void checkSonar(int period, int refresh){
    long prev= millis();
    long timer = 0; 
   while( timer < period){
        timer += (millis() - prev);
        prev = millis();
        lcd.clear(); 
        lcd.print(String("L")+String(getSonarLeft())+
                  String(" R")+String(getSonarRight())+
                  String(" B")+String(getSonarLeftBack())+
                  String(" F")+String(getSonarFront()));
        delay(refresh);
    }
  
}
/**
 * BUG: some tim ecannot use the same function again the  bot jsut stio 
 */
void setup() {
 // SabertoothSimplified motordriver(Serial3); 
  // put your setup code here, to run once:
   // sets the LCD's rows and colums:
 lcd.begin();
 lcd.backlight(); //backlight is now ON
  setSonarOffset(sonarOffset);//update offset 7/23/18

  dinit();
  Serial.begin(115200);
   checkSonar(5000,100);
}

void loop() {
  // put your main code here, to run repeatedly:
  driveto(10);
  delay(2000);
  driveto(10);
    delay(2000);
  driveto(10);
    delay(2000);
  driveto(10);
    delay(2000);
  driveto(5);
  Serial.println("hello");
  while(1);
}
