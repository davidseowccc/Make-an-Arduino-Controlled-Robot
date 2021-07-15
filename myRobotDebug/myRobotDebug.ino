/**********************************************************
MyRobotDebug.ino

Simple debug example using Processing debug monitor
This version displays values from the line and distance sensors

***********************************************************/
#include "robotDefines.h"  // global defines

const byte pingPin = 10; // Distance sensor connected to digital pin 10

enum {DATA_start, DATA_LEFT, DATA_CENTER, DATA_RIGHT,DATA_DRIFT,DATA_DISTANCE,
     DATA_nbrItems};

char* labels[]= {"","Left Line","Center Line","Right Line","Drift","Distance"};
int minRange[]= { 0,          0,            0,           0,  -1023,        0 };  
int maxRange[]= { 0,       1023,         1023,        1023,   1023,       144};  
                
// Setup runs at startup and is used configure pins and init system variables
void setup()
{
  Serial.begin(9600);  
  while(!Serial);  // only needed for leonardo
  
  dataDisplayBegin(DATA_nbrItems, labels, minRange, maxRange );
  Serial.println("Ready");
}

void loop()
{
  lineSense();
  int distance = pingGetDistance(pingPin);
  sendData(DATA_DISTANCE, distance); // send distance
}

/****************************
   Line  Sensor code   
****************************/
// defines for locations of sensors
const int SENSE_LINE_LEFT  = 0;
const int SENSE_LINE_RIGHT = 1;
const int SENSE_LINE_CENTER = 2;

//returns drift - 0 if over line, minus value if left, plus if right
int lineSense()
{
  int leftVal = analogRead(SENSE_LINE_LEFT);
  int centerVal = analogRead(SENSE_LINE_CENTER);
  int rightVal = analogRead(SENSE_LINE_RIGHT);

  sendData(DATA_LEFT, leftVal);      // send left sensor value
  sendData(DATA_CENTER, centerVal);  // send center sensor value
  sendData(DATA_RIGHT, rightVal);    // send right sensor values

  int leftSense = centerVal - leftVal;
  int rightSense = rightVal - centerVal;
  int drift = leftVal- rightVal   ;
  
  sendData(DATA_DRIFT, drift);    // send drift sensor values
  
  return drift;
}

// Returns the distance in inches
// this will return 0 if no ping sensor is connected or 
// the distance is greater than around 10 feet
int pingGetDistance(int pingPin)
{
  long duration, cm;

  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin, LOW);

  pinMode(pingPin, INPUT);
  duration = pulseIn(pingPin, HIGH, 20000); // if a pulse does not arrive in 20 ms then 
                                            // the ping sensor is not connected
  if(duration >=20000)
    return 0;
  // convert the time into a distance
  cm = duration / 29 / 2;
  return (cm * 10) / 25 ; // convert cm to inches
}