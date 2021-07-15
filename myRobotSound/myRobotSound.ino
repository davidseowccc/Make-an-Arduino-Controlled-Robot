/**********************************************************
MyRobotSound.ino

Robot moves when a sound level exceeds a threshold
Based on Recipe 6.7 from Arduino Cookbook

Copyright Michael Margolis 20 July 2012

***********************************************************/

#include <AFMotor.h>  // adafruit motor shield library
#include "RobotMotor.h"    // 2wd or 4wd motor library

#include "robotDefines.h"  // global defines

const int analogInPin = 5;      // analog pin the sensor is connected to

const int middleValue = 512;      //the middle of the range of analog values
const int numberOfSamples = 128;  //how many readings will be taken each time

int sample;                       //the value read from microphone each time
long signal;                      //the reading once you have removed DC offset
long averageReading;              //the average of that loop of readings

long runningAverage=0;          //the running average of calculated values
const int averagedOver= 16;     //how quickly new values affect running average
                                //bigger numbers mean slower

const int threshold=400;        //at what level the robot will move

int speed = 50;

// Setup runs at startup and is used configure pins and init system variables
void setup()
{
  Serial.begin(9600);
  blinkNumber(8); // open port while flashing. Needed for Leonardo only  
  
  motorBegin(MOTOR_LEFT); 
  motorBegin(MOTOR_RIGHT); 
}

void loop()
{
  int level = getSoundLevel();
  if (level > threshold)         //is level more than the threshold ?
  {
     motorForward(MOTOR_LEFT, speed);    
     motorForward(MOTOR_RIGHT, speed);  
  }else
  {
     motorStop(MOTOR_LEFT);    
     motorStop(MOTOR_RIGHT);  
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

int getSoundLevel()
{
  long sumOfSquares = 0;
  for (int i=0; i<numberOfSamples; i++) { //take many readings and average them
    sample = analogRead(analogInPin);     //take a reading
    signal = (sample - middleValue);      //work out its offset from the center
    signal *= signal;                     //square it to make all values positive
    sumOfSquares += signal;               //add to the total
  }
  averageReading = sumOfSquares/numberOfSamples;     //calculate running average
  runningAverage=(((averagedOver-1)*runningAverage)+averageReading)/averagedOver;

  return runningAverage;
}

