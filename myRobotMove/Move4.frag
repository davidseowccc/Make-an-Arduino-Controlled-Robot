/************* high level movement functions ****************/

//moves in the given direction at the current speed for the given duration in milliseconds
void timedMove(int direction, int duration)
{
  Serial.print("Timed move ");
  if(direction == MOV_FORWARD) {
    Serial.println("forward");
    moveForward();    
  }
  else if(direction == MOV_BACK) {
    Serial.println("back");
    moveBackward();     
  }
  else
    Serial.println("?");
    
  movingDelay(duration);
  moveStop();
}

// check for obstacles while delaying the given duration in ms
void movingDelay(long duration)
{
  long startTime = millis();
  while(millis() - startTime < duration) {
     // function in Look module checks for obstacle in direction of movement 
     if(checkMovement() == false) { 
        if( moveState != MOV_ROTATE) // rotate is only valid movement
        {
            Serial.println("Stopping in moving Delay()"); 
            moveBrake(); 
        }
     }  
  }  
}