/****************************************************************************
myRobotLineDisplay.ino

Based on myRobotLine - adds code to display variables in Processing

Created by Michael Margolis 15 July 2012

****************************************************************************/

#include <AFMotor.h>  // adafruit motor shield library
#include "RobotMotor.h"    // 2wd or 4wd motor library

#include "robotDefines.h"  // these were the global defines from myRobot

int speed = MIN_SPEED; // speed in percent when moving along a straight line

enum {DATA_start,DATA_LEFT,DATA_CENTER,DATA_RIGHT,DATA_DRIFT,DATA_L_SPEED,DATA_R_SPEED, DATA_DAMPING,
     DATA_nbrItems};

 char* labels[] = 
       {"", "Left Line", "Center Line", "Right Line","Drift", "Left Speed", "Right Speed", "Damping"};

int minRange[] = 
              {0,          0,             0,            0,      -1023,         0,    0,    0 };  

int maxRange[] =  
              {0,        1023,         1023,         1023,        1023,       100,   100,  40};  
              

/// Setup runs at startup and is used configure pins and init system variables
void setup()
{
  Serial.begin(9600);    
  blinkNumber(8); // open port while flashing. Needed for Leonardo only  

  lookBegin();  /// added Look tab
  moveBegin();  /// added Move tab  
  lineSenseBegin();    // initialize sensors 
  dataDisplayBegin(DATA_nbrItems, labels, minRange, maxRange );
  Serial.println("Ready");  
}

void loop()
{
  int drift = lineSense();
  lineFollow(drift, speed);
  delay(1);
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
void lineSenseBegin()
{

}

//returns drift - 0 if over line, minus value if left, plus if right
int lineSense()
{
  int leftVal = analogRead(SENSE_IR_LEFT);
  int centerVal = analogRead(SENSE_IR_CENTER);
  int rightVal = analogRead(SENSE_IR_RIGHT);

  sendData(DATA_LEFT, leftVal);      // send left sensor value
  sendData(DATA_CENTER, centerVal);  // send center sensor value
  sendData(DATA_RIGHT, rightVal);    // send right sensor values

  int leftSense = centerVal - leftVal;
  int rightSense = rightVal - centerVal;
  int drift = leftVal- rightVal   ;
  
  sendData(DATA_DRIFT, drift);    // send drift sensor values
  
  return drift;
}

int lineFollow(int drift, int speed)
{
//int damping =  5; //1 is most sensitive, range 1 to 1023)  
  int damping = map(analogRead(5), 0,1023, 1, 40);
  sendData(DATA_DAMPING, damping); 
  
  int leftSpeed   =  constrain(speed - (drift / damping), 0, 100);
  int rightSpeed  =  constrain(speed + (drift / damping), 0, 100);
  
  sendData(DATA_L_SPEED, leftSpeed);   // send left motor speed
  sendData(DATA_R_SPEED, rightSpeed);  // send right motor speed 
  
  motorForward(MOTOR_LEFT, leftSpeed);
  motorForward(MOTOR_RIGHT, rightSpeed);  
}
