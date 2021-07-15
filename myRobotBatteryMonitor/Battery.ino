// code to monitor battery voltage

/******************************************************************
 * LED starts flashing when volage drops below warning level 
 * mark space ratio increses from 10% to 50% as voltage decreses from warning to critical
 * robot shuts down when battery below critical and led flashes SOS
 * 
 * LED mark space ratio changes from 10% to 90% as voltage increases to full
 *****************************************************************/


// thresholds are the cell millivolts times number of cells
const int batteryFull    =  1500 * 5; // threshold for battery is low warning
const int batteryWarning =  1100 * 5; // threshold for battery is low warning
const int batteryCritical=  1000 * 5; // threshold to shut down robot

int batteryMonitorPin;    // analog pin to monitor
int chargerDetectPin =-1; // pin goes open circuit when charger connected, default is no pin
int blinkPin;             // led pin to flash


void batteryBegin(int monitorPin, int ledPin)
{
  batteryMonitorPin = monitorPin;
  blinkPin = ledPin;
  pinMode(blinkPin, OUTPUT);
}

// version for charger detection
void  batteryBegin(int monitorPin, int ledPin, int chargerPin)
{
  batteryBegin(monitorPin, ledPin);
  chargerDetectPin = chargerPin;
  pinMode(chargerDetectPin, INPUT_PULLUP); // connect pull-up resistor   
}

// indicates battery status using the given LED
void  batteryCheck()
{  
  int mv = batteryMv(batteryMonitorPin); // get battery level in millivolts
  Serial.print("mv="); Serial.print(mv); 
  if(chargerDetectPin >=0 &&  digitalRead(chargerDetectPin) == HIGH)
  {
    // here if charger detect is enabled and charger plugged in 
    while( digitalRead(chargerDetectPin) == HIGH) // while charger is plugged in
    {
        moveStop();       
        mv =  batteryMv(batteryMonitorPin); // get battery level in millivolts
        Serial.print(", charger detected, voltage=");
        Serial.println(mv); Serial.println(", percent="); 
        int percent = map(mv, batteryCritical, batteryFull, 50, 100);
        percent = constrain(percent, 0, 100);
        Serial.println(percent); 
        flash(percent, blinkPin);  
    }
  }
  else
  {
 
    if(mv < batteryCritical)
    {
      Serial.println("Critical");
      // shut down the robot
      moveStop(); 
      while(1) { 
        flashCritical(blinkPin);
        // check of the charger is plugged in
        if(chargerDetectPin >=0 && digitalRead(chargerDetectPin) == HIGH) 
           return; // exit if charging        
       
        delay(5000);
     }
    }
    else if (mv < batteryWarning)
    {  
      int percent = map(mv, batteryCritical, batteryWarning, 10, 50);
      flash(percent, blinkPin);   
    }    
  } 
  delay(1000); 
  Serial.println();
}

// return the voltge on the given pin in millivolts
// see text for voltage divider resistor values
int  batteryMv(int pin )
{
#if defined(__AVR_ATmega32U4__) // is this a Leonardo board?
  const long INTERNAL_REFERENCE_MV = 2560; // leo reference is 2.56 volts
#else
  const long INTERNAL_REFERENCE_MV = 1100; // ATmega328 is 1.1 volts
#endif  
  const float R1 = 18.0;  // voltge dividier resistors values, see text
  const float R2 = 2.2;    
  const float DIVISOR = R2/(R1+R2); 

  analogReference(INTERNAL);      // set reference to internal (1.1V)
  analogRead(pin);  // allow the ADC to settle
  delay(10);

  int value = 0;
  for(int i=0; i < 8; i++) {    
    value = value + analogRead(pin);
  }
  value  = value / 8; // get the average of 8 readings
  int mv = map(value, 0,1023, 0, INTERNAL_REFERENCE_MV / DIVISOR );

  analogReference(DEFAULT); // set the reference back to default (Vcc)
  analogRead(pin); // just to let the ADC settle ready for next reading
  delay(10); // allow reference to stabalise

  return mv;
}

// flashes SOS in morse code
void flashCritical(int pin)
{
  for(int i=0; i< 3; i++)
    flash(20, pin);
  for(int i=0; i< 3; i++)
    flash(60, pin);     
  for(int i=0; i< 3; i++)
    flash(20, pin); 
}


// percent is the percent of on time time (duty cycle)
void flash(int percent, int pin)
{
  Serial.print(", flash percent="); Serial.println(percent); 
  const int duration = 1000;  
  // Blink the LED
  digitalWrite( pin, HIGH);
  int onTime = map(percent, 0, 100, 0, duration);
  delay(onTime);  
  digitalWrite( pin, LOW);
  delay(duration - onTime);   
}

