void moveRotate(int angle)
{
  Serial.print("Rotating ");  Serial.println(angle);
  if(angle < 0)
  {
    Serial.println(" (left)"); 
    motorReverse(MOTOR_LEFT,  moveSpeed); 
    motorForward(MOTOR_RIGHT, moveSpeed);  
    angle = -angle;
	changeMoveState(MOV_ROTATE);
  }
  else if(angle > 0)
  {
    Serial.println(" (right)");
    motorForward(MOTOR_LEFT,  moveSpeed);
    motorReverse(MOTOR_RIGHT, moveSpeed);
    changeMoveState(MOV_ROTATE);
  }  
  int ms = rotationAngleToTime(angle, moveSpeed);
  movingDelay(ms); 
  moveBrake();   
}

// return the time in milliseconds to turn the given angle at the given speed
long rotationAngleToTime( int angle, int speed)
{
int fullRotationTime; // time to rotate 360 degrees at given speed

  if(speed < MIN_SPEED)
    return 0; // ignore speeds slower then the first table entry

  angle = abs(angle);

  if(speed >= 100)
    fullRotationTime = rotationTime[NBR_SPEEDS-1]; // the last entry is 100%
  else
 { 
    int index = (speed - MIN_SPEED) / SPEED_TABLE_INTERVAL; // index into speed and time tables
    int t0 =  rotationTime[index];
    int t1 = rotationTime[index+1];    // time of the next higher speed 
    fullRotationTime = map(speed,  speedTable[index],  speedTable[index+1], t0, t1);  
   // Serial.print("index= ");  Serial.print(index); Serial.print(", t0 = "); Serial.print(t0);
   // Serial.print(", t1 = ");  Serial.print(t1); 
 }
 // Serial.print(" full rotation time = ");  Serial.println(fullRotationTime);
  long result = map(angle, 0,360, 0, fullRotationTime);
  return result; 
}



// rotate the robot from MIN_SPEED to 100% increasing by SPEED_TABLE_INTERVAL
void calibrateRotationRate(int direction, int angle)
{  
  Serial.print(locationString[direction]);
  Serial.println(" calibration" );    
  for(int speed = MIN_SPEED; speed <= 100; speed += SPEED_TABLE_INTERVAL)
  { 
    delay(1000);
    //blinkNumber(speed/10);   
 
    if( direction == DIR_LEFT)
    {    // rotate left
      motorReverse(MOTOR_LEFT,  speed); 
      motorForward(MOTOR_RIGHT, speed);  
    }
    else if( direction == DIR_RIGHT)
    {    // rotate right
      motorForward(MOTOR_LEFT,  speed);
      motorReverse(MOTOR_RIGHT, speed);
    }
    else
       Serial.println("Invalid direction");
    

    int time = rotationAngleToTime(angle, speed);

    Serial.print(locationString[direction]);
    Serial.print(": rotate ");           Serial.print(angle);
    Serial.print(" degrees at speed ");  Serial.print(speed);
    Serial.print(" for ");               Serial.print(time);
    Serial.println("ms");           
    delay(time); 
    motorStop(MOTOR_LEFT);
    motorStop(MOTOR_RIGHT);  
    delay(2000); // two second delay between speeds
  }    
}
