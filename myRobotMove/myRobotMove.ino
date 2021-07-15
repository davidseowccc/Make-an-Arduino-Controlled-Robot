/*************************************************************************
myRobotMove.ino

Robot sketch based on myRobot with higher level movement functions

Michael Margolis 28 May 2012
**************************************************************************/

#include <AFMotor.h>  // adafruit motor shield library
#include "RobotMotor.h"    // 2wd or 4wd motor library

#include "robotDefines.h"  // global defines

// Setup runs at startup and is used configure pins and init system variables
void setup()
{
  Serial.begin(9600);
  blinkNumber(8); // open port while flashing. Needed for Leonardo only  
  moveBegin();        /// added Move tab  
  irSensorBegin();    // initialize sensors 

  irSensorBegin();    // initialize sensors 
  while( irSensorDetect(SENSE_IR_LEFT) == false &&
         irSensorDetect(SENSE_IR_RIGHT) == false)
     ; // wait for at least one sensor to detect a reflection  
  Serial.println("Ready");  
}

void loop()
{
  Serial.println("Waiting for a reflection from either sensor");
  while (lookForObstacle(OBST_FRONT_EDGE) == false)
     ; // wait while one or both sensor to detects a reflection 

   moveSetSpeed(MIN_SPEED + 10) ;  // Run at 10% above minimum speed
   // call a function when reflection blocked on left side
   if(lookForObstacle(OBST_LEFT_EDGE) == true)   { 
       moveRotate(-360);  // one CCW rotation
   }
   // as above for right sensor
   if(lookForObstacle(OBST_RIGHT_EDGE) == true)   {   
     moveRotate(360);  // one CW rotation
   }  
}

// function to indicate numbers by flashing the built-in LED
void blinkNumber( byte number) {
   pinMode(LED_PIN, OUTPUT); // enable the LED pin for output
   while(number--) {
     digitalWrite(LED_PIN, HIGH); delay(100);
     digitalWrite(LED_PIN, LOW);  delay(400);
   }
}

/**********************
 code to look for obstacles
**********************/

// returns true if the given obstacle is detected
boolean lookForObstacle(int obstacle)
{
  switch(obstacle) {
     case  OBST_FRONT_EDGE: return irEdgeDetect(DIR_LEFT) &&
                                   irEdgeDetect(DIR_RIGHT); 
     case  OBST_LEFT_EDGE:  return irEdgeDetect(DIR_LEFT); 
     case  OBST_RIGHT_EDGE: return irEdgeDetect(DIR_RIGHT);      
  }
  return false; 
}

// function to check if robot can continue moving when taking evasive action
// returns true if robot is not blocked when moving to avoid obstacles
// this 'placeholder' version always returns true
boolean checkMovement()
{
  return true; 
}
