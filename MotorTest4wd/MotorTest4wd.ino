
/*******************************************
* MotorTest4wd.ino
* Initial motor test for 4WD 
* robot rotates clockwise 
*  (Left motors driven forward, right backward)

* Michael Margolis 24 July 2012
********************************************/
const int LED_PIN = 13;
const int speed = 60; // percent of maximum speed

#include <AFMotor.h>  // adafruit motor shield library (modified my mm)
AF_DCMotor Motor_Left_Front(4, MOTOR34_1KHZ);   // Motor 4 
AF_DCMotor Motor_Right_Front(3, MOTOR34_1KHZ);  // Motor 3
AF_DCMotor Motor_Left_Rear(1, MOTOR12_1KHZ);    // Motor 1 
AF_DCMotor Motor_Right_Rear(2, MOTOR12_1KHZ);   // Motor 2

int pwm;

void setup()
{
  Serial.begin(9600);
  blinkNumber(8); // open port while flashing. Needed for Leonardo only  
   
  // scale percent into pwm range (0-255) 
  pwm= map(speed, 0,100, 0,255);  
  Motor_Left_Front.setSpeed(pwm);
  Motor_Right_Front.setSpeed(pwm);
  Motor_Left_Rear.setSpeed(pwm);
  Motor_Right_Rear.setSpeed(pwm);
}

// run over and over
void loop()
{  
  Serial.println("rotate cw");
  Motor_Left_Front.run(FORWARD);
  Motor_Left_Rear.run(FORWARD);
  
  Motor_Right_Front.run(BACKWARD);
  Motor_Right_Rear.run(BACKWARD);

  delay(5000); // run for 5 seconds
  Serial.println("stopped");
  Motor_Left_Front.run(RELEASE);  // stop the motors
  Motor_Right_Front.run(RELEASE);
  Motor_Left_Rear.run(RELEASE);  // stop the motors
  Motor_Right_Rear.run(RELEASE);

  delay(5000); // stop for 5 seconds
}

// function to indicate numbers by flashing the built-in LED
void blinkNumber( byte number) {
   pinMode(LED_PIN, OUTPUT); // enable the LED pin for output
   while(number--) {
     digitalWrite(LED_PIN, HIGH); delay(100);
     digitalWrite(LED_PIN, LOW);  delay(400);
   }
}
