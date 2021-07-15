
/*******************************************
* MotorTest2wd.ino
* Initial motor test for 2WD - robot rotates clockwise 
* Left motor driven forward, right backward

* Michael Margolis 24 July 2012, updated comments 3 Nov 2012
********************************************/
const int LED_PIN = 13;
const int speed = 60; // percent of maximum speed

#include <AFMotor.h>  // adafruit motor shield library (modified my mm)
AF_DCMotor Motor_Left(1, MOTOR12_1KHZ);   // Motor 1 
AF_DCMotor Motor_Right(2, MOTOR12_1KHZ);  // Motor 2

int pwm;

void setup()
{
  Serial.begin(9600);
  blinkNumber(8); // open port while flashing. Needed for Leonardo only  
  
  // scale percent into pwm range (0-255)
  pwm= map(speed, 0,100, 0,255);  
  Motor_Left.setSpeed(pwm);
  Motor_Right.setSpeed(pwm);
}

// run over and over
void loop()
{  
  Serial.println("rotate cw");
  Motor_Left.run(FORWARD);
  Motor_Right.run(BACKWARD);
  delay(5000); // run for 5 seconds
  Serial.println("pause");
  Motor_Left.run(RELEASE);  // stop the motors
  Motor_Right.run(RELEASE);
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
