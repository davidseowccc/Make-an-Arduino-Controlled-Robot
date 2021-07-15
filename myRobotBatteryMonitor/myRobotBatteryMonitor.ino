/******************************************************************************
myRobotBatteryMonitor.ino

sketch to demonstrate battery voltage monitoring
based on myRobotWander

Robot wanders using forward scanning for obstacle avoidance
LED blinks when battery runs low, robot goes to sleep when battery is critical.

Created by Michael Margolis 22 July 2012
******************************************************************************/
#include "robotDefines.h"  // global defines

#include <AFMotor.h>  // adafruit motor shield library
#include "RobotMotor.h"    // 2wd or 4wd motor library

const int ledPin = 13;          // onboard LED 
const int alogBatteryPin = 5;   // input on analog 5
const int chargerDetectedPin = 2; // digital pin 2

// Setup runs at startup and is used configure pins and init system variables
void setup()
{
  Serial.begin(9600);
  blinkNumber(8); // open port while flashing. Needed for Leonardo only  
  
  lookBegin(); 
  moveBegin();
  //batteryBegin(alogBatteryPin, ledPin); 
  batteryBegin(alogBatteryPin, ledPin, chargerDetectedPin); 
  
  pinMode(ledPin, OUTPUT);
  Serial.println("Ready");    
}

void loop()
{
//  roam();
  batteryCheck(); 
}

// function to indicate numbers by flashing the built-in LED
void blinkNumber( byte number) {
   pinMode(LED_PIN, OUTPUT); // enable the LED pin for output
   while(number--) {
     digitalWrite(LED_PIN, HIGH); delay(100);
     digitalWrite(LED_PIN, LOW);  delay(400);
   }
}


