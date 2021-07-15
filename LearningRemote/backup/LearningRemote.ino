/*
 * LearningRemote.cpp
 *
 * captures remote commands and stores decoded values in EEPROM
 */

#include <EEPROM.h>
#include <IRremote.h>       // IR remote control library

const int irPin = 14;
const long NO_KEY = -1; 
const long TIMEOUT = 5000; //max number of milliseconds to wait for a key (5 secs)
const int KEYCOUNT = 7; // the number of key codes supported
// the following is for a temp user interface for learning mode                               
char * remoteKeyNames[KEYCOUNT] = {"Forward", "Back", "Left",  "Right", "PivotCCW, PivotCW", "Halt" }; 
// not used: Slower, Faster

const char keyId[2] = {'i','r'};

const int eepromKeyMapHeader = 32;
const int eepromKeyMapHeaderSize = 3;
const int eepromKeyMap = eepromKeyMapHeader + eepromKeyMapHeaderSize ;  

long irKeyMap[KEYCOUNT] ; // you can define default values here


IRrecv irrecv(irPin);     // create the IR receive object
decode_results results;   // ir data goes here  


void setup()
{
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the ir receiver
  learnKeycodes();
  showRemoteCodes();
}


void loop()
{
 long code = getIrKeycode(TIMEOUT);
 int key = mapCodeToKey(code);
 if( key >= 0)
    Serial.println(remoteKeyNames[key]); 
}

// wait up to timeout milliseconds for a key
long getIrKeycode(long timeout)
{

  flushKeys();
    
  long key = -1;
  unsigned long startTime = millis();
  while( millis() - startTime < timeout )
  {
    if( irrecv.decode(&results) ) { 
      key = results.value;
      //Serial.println(key, HEX);
      irrecv.resume(); // Receive the next value   
      if(key != -1 ) {
        break;
      }
    }  
  }
  return key; 
}

//clear the buffer
void flushKeys()
{
  while(irrecv.decode(&results)) 
    irrecv.resume();
 results.value = -1;    
}


// returns the nuber of keys stored in eeprom
// returns 0 if no keys stored
int getStoredKeycount()
{
  if( EEPROM.read(eepromKeyMap) == keyId[0] &&  EEPROM.read(eepromKeyMap+1) == keyId[1] )
    return EEPROM.read( eepromKeyMap+2);  
}

// get remote control codes

void learnKeycodes()
{
  Serial.println("Ready to learn remote codes");      
  for(int i = 0; i < KEYCOUNT;)
  {
    delay(100);
    Serial.println();
    Serial.print("press remote key for ");
    Serial.print( remoteKeyNames[i]);    
    long key = getIrKeycode(TIMEOUT);
    long k = -1;
    if( key != -1)
    {
       // Serial.println(key, HEX);
        Serial.println(" release key ...");
        do {          
             k = getIrKeycode(1000);
           //  Serial.println(k, HEX);                  
        }
        while( k == key);
        Serial.println(" press key again");       
        if(getIrKeycode(TIMEOUT) == key)
        {
          Serial.print(" -> usi4ng 0x");
          Serial.print( key, HEX);
          Serial.print(" for ");
          Serial.println(remoteKeyNames[i]);
          irKeyMap[i] = key;  
          i++;
        }
        else
          Serial.println("Keys did not match");
    }
  }     
  Serial.println("Learning complete\n");  
  saveRemoteCodes();
}

// for debug only
void showRemoteCodes()
{
  Serial.println("Remote codes are:");
  for(int i = 0; i < KEYCOUNT; i++)
  {
    Serial.print("key ");
    Serial.print(  remoteKeyNames[i]);
    Serial.print(" = 0x");
    Serial.println(irKeyMap[i], HEX); 
  }  
}

void saveRemoteCodes()
{
  byte *cfgPtr = (byte*)irKeyMap;
  for(int i=0; i < sizeof(irKeyMap); i++)  
    EEPROM.write( eepromKeyMap + i, cfgPtr[i] );      
}

void restoreRemoteCodes()
{
  byte *cfgPtr = (byte*)irKeyMap;
  for(int i=0; i < sizeof(irKeyMap); i++)  
    cfgPtr[i] = EEPROM.read(  eepromKeyMap + i );          
}


// returns a key index or NO_KEY ( -1) if no digit received
int getRemoteKeypress()
{
  int ret = NO_KEY; // default 
  if (irrecv.decode(&results)) 
  {
    //Serial.println(results.value, HEX);
#ifdef IR_DEBUG      
    dumpRawRemoteData(&results);
#endif     
    irrecv.resume(); // Receive the next value  
    ret = mapCodeToKey(results.value);
  }    
  return ret;
}

void dumpRawRemoteData(decode_results *results) {
  int count = results->rawlen;
  if(results->bits == 0)
    return;

  if (results->decode_type == UNKNOWN) {
    Serial.println("Could not decode message");
  } 
  else {
    if (results->decode_type == NEC) {
      Serial.print("Decoded NEC: ");
    } 
    else if (results->decode_type == SONY) {
      Serial.print("Decoded SONY: ");
    } 
    else if (results->decode_type == RC5) {
      Serial.print("Decoded RC5: ");
    } 
    else if (results->decode_type == RC6) {
      Serial.print("Decoded RC6: ");
    }
    Serial.print("0x");
    Serial.print(results->value, HEX);
    Serial.print(" (");
    Serial.print(results->bits, DEC);
    Serial.print(" bits)");
  }
#ifdef RAW_REMOTE_CTRL_DEBUG   
  Serial.print("Raw (");
  Serial.print(count, DEC);
  Serial.print("): ");

  for (int i = 0; i < count; i++) {
    if ((i % 2) == 1) {
      Serial.print(results->rawbuf[i]*USECPERTICK, DEC);
    } 
    else {
      Serial.print(-(int)results->rawbuf[i]*USECPERTICK, DEC);
    }
    Serial.print(" ");
  }
  Serial.println("");
#endif;  
}

// converts an ir  remote code to a logical key code (or NO_KEY if no digit received)
int mapCodeToKey(long code)
{

  int key = NO_KEY;  // no key pressed
  if( code != REPEAT) // NEC protocol sends this when a button is held down
  {  
    for( int i=0; i < KEYCOUNT; i++)
    {
      if( code == irKeyMap[i])
      {
        key = i;   
        break;
      }
    }
  }
#ifdef IR_DEBUG
  if( code != -1)
  {
    Serial.print("Code  ");  
    Serial.print(code,HEX);  
    Serial.print(" mapped to key ");  
    Serial.println(key);  
  }
#endif   
  return key;
}



