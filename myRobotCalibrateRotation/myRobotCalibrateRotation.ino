/**********************************************************
MyRobotCalibrateRotation.ino
***********************************************************/
// include motor libraries
#include <AFMotor.h>     // adafruit motor shield library
#include <RobotMotor.h>  // 2wd or 4wd motor library

// Setup runs at startup and is used configure pins and init system variables
void setup()
{
  motorBegin(MOTOR_LEFT); 
  motorBegin(MOTOR_RIGHT); 
  calibrateSpeed();
}

void loop()
{ 
}

void calibrateSpeed()
{
  for(int speed = MIN_SPEED; speed <= 100; speed += 10)
  {
     // rotate robot left for 1 second 
      motorReverse(MOTOR_LEFT,  speed); 
      motorForward(MOTOR_RIGHT, speed);  
      delay(1000); // delay 1 second
      motorStop(MOTOR_LEFT); 
      motorStop(MOTOR_RIGHT);  
      
      delay(3000);  // wait 3 seconds
      
      // rotate robot right for 1 second
      motorReverse(MOTOR_RIGHT,  speed); 
      motorForward(MOTOR_LEFT, speed);  
      delay(1000);  // delay 1 second
      motorStop(MOTOR_LEFT); 
      motorStop(MOTOR_RIGHT);  
      delay(3000); // wait 3 seconds
  } 
}

