/*************************************
 Drive: mid level movement functions
*************************************/

int moveState = MOV_STOP;   // what robot is doing
 
int  moveSpeed   = 0; // move speed stored here (0-100%)
int  speedIncrement = 10; // percent to increase or decrease speed

void moveBegin()
{
   motorBegin(MOTOR_LEFT); 
   motorBegin(MOTOR_RIGHT); 
   moveStop();
}

void moveLeft()
{
  motorForward(MOTOR_LEFT,  0);
  motorForward(MOTOR_RIGHT, moveSpeed);
  changeMoveState(MOV_LEFT);  
}

void moveRight()
{
  motorForward(MOTOR_LEFT,  moveSpeed);
  motorForward(MOTOR_RIGHT, 0);
  changeMoveState(MOV_RIGHT);  
}

void moveStop()
{
  motorStop(MOTOR_LEFT);
  motorStop(MOTOR_RIGHT);
  changeMoveState(MOV_STOP);  
}

void moveBrake()
{
  motorBrake(MOTOR_LEFT);
  motorBrake(MOTOR_RIGHT);
  changeMoveState(MOV_STOP);  
}

void moveBackward()
{
  motorReverse(MOTOR_LEFT, moveSpeed);
  motorReverse(MOTOR_RIGHT, moveSpeed);  
  changeMoveState(MOV_BACK);  
}

void moveForward()
{
  motorForward(MOTOR_LEFT,  moveSpeed);
  motorForward(MOTOR_RIGHT, moveSpeed);
  changeMoveState(MOV_FORWARD);  
}

void moveSetSpeed(int speed)
{
  motorSetSpeed(MOTOR_LEFT, speed) ;
  motorSetSpeed(MOTOR_RIGHT, speed) ;
  moveSpeed = speed; // save the value
}