/******************************************************************************
myRobotEdge.ino

Robot sketch to move within area bordered by a non-reflective line

Michael Margolis 7 July 2012
******************************************************************************/
#include <AFMotor.h>  // adafruit motor shield library
#include "RobotMotor.h"    // 2wd or 4wd motor library

#include "robotDefines.h"  // these were the global defines from myRobot

/// Setup runs at startup and is used configure pins and init system variables
void setup()
{
  Serial.begin(9600);    
  blinkNumber(8); // open port while flashing. Needed for Leonardo only  
  
  lookBegin();  /// added Look tab
  moveBegin();  /// added Move tab  
  Serial.println("Ready");  
}

void loop()
{
  /// code for roaming around and avoiding obstacles
  if( lookForObstacle(OBST_FRONT_EDGE) == true)
  {
     Serial.println("both sensors detected edge");
     timedMove(MOV_BACK, 300);
     moveRotate(120);
     while(lookForObstacle(OBST_FRONT_EDGE) == true )
        moveStop(); // stop motors if still over cliff
  }
  else if(lookForObstacle(OBST_LEFT_EDGE) == true)
  {
     Serial.println("left sensor detected edge");
     timedMove(MOV_BACK, 100);
     moveRotate(30);
  }
  else if(lookForObstacle(OBST_RIGHT_EDGE) == true)  
  {
     Serial.println("right sensor detected edge");
     timedMove(MOV_BACK, 100);
      moveRotate(-30); 
  }
  else
  {
    moveSetSpeed(MIN_SPEED);
    moveForward();    
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

