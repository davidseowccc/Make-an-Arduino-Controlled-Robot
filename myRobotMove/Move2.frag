void moveSlower(int decrement)
{
   Serial.print(" Slower: "); 
   if( moveSpeed >= speedIncrement + MIN_SPEED)
     moveSpeed -= speedIncrement;     
   else moveSpeed = MIN_SPEED;
   moveSetSpeed(moveSpeed); 
}

void moveFaster(int increment)
{
  Serial.print(" Faster: ");
  moveSpeed += speedIncrement; 
  if(moveSpeed > 100)
     moveSpeed = 100;
  moveSetSpeed(moveSpeed);      
}

int moveGetState()
{
 return moveState;  
}

// this is the low level movement state.
// it will differ from the command state when the robot is avoiding obstacles
void changeMoveState(int newState)
{
  if(newState != moveState)
  {
    Serial.print("Changing move state from "); Serial.print( states[moveState]);
    Serial.print(" to "); Serial.println(states[newState]);
    moveState = newState;
  } 
}