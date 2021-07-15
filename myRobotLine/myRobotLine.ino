/******************************************************************************
myRobotLine.ino

Robot sketch to follow lines

Michael Margolis 7 July 2012
******************************************************************************/

#include <AFMotor.h>  // adafruit motor shield library
#include "RobotMotor.h"    // 2wd or 4wd motor library

#include "robotDefines.h"  // these were the global defines from myRobot

int speed = MIN_SPEED; // speed in percent when moving along a straight line

/// Setup runs at startup and is used configure pins and init system variables
void setup()
{
  Serial.begin(9600); 
  blinkNumber(8); // open port while flashing. Needed for Leonardo only  
  
  lookBegin();  /// added Look tab
  moveBegin();  /// added Move tab  
  lineSenseBegin();    // initialize sensors 
  Serial.println("Ready");  
}

void loop()
{
  int drift = lineSense();
  lineFollow(drift, speed);
}

// function to indicate numbers by flashing the built-in LED
void blinkNumber( byte number) {
   pinMode(LED_PIN, OUTPUT); // enable the LED pin for output
   while(number--) {
     digitalWrite(LED_PIN, HIGH); delay(100);
     digitalWrite(LED_PIN, LOW);  delay(400);
   }
}

/****************************
   Line  Sensor code   
****************************/

int damping =  5; //1 is most sensitive, range 1 to 1023)

void lineSenseBegin()
{

}

//returns drift - 0 if over line, minus value if left, plus if right
int lineSense()
{
  int leftVal = analogRead(SENSE_IR_LEFT);
  int centerVal = analogRead(SENSE_IR_CENTER);
  int rightVal = analogRead(SENSE_IR_RIGHT);

  int leftSense = centerVal - leftVal;
  int rightSense = rightVal - centerVal;
  int drift = rightVal - leftVal ;
  return drift;
}

int lineFollow(int drift, int speed)
{
  int leftSpeed   =  constrain(speed - (drift / damping), 0, 100);
  int rightSpeed  =  constrain(speed + (drift / damping), 0, 100);
  
  motorForward(MOTOR_LEFT, leftSpeed);
  motorForward(MOTOR_RIGHT, rightSpeed);  
}
