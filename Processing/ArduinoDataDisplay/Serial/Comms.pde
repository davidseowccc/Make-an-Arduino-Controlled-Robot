
//  Non-android serial code for ArduinoDataDisplay

import processing.serial.*;

short portIndex = 0;  // select the com port, 0 is the first port
Serial myPort;  // Create object from Serial class

void initComms(){
  String portName = Serial.list()[portIndex];
  println(Serial.list());
  println(" Connecting to -> " + portName) ;
  myPort = new Serial(this, portName, 9600); 
}

String  commsPortString() {
  return "(" + Serial.list()[portIndex] + ")" ; 
}  

String commsGetMessage() {
   while (myPort.available () > 0) {
    try {
      message = myPort.readStringUntil(10); 
      if (message != null) {
        print(message); 
        int row = 0;
        
        String [] data  = message.split(","); // Split the CSV message     
        if ( data[0].equals("Label") ) { // check for label header       
          row =  Integer.parseInt(data[1]); 
          labelList.set(row,data[2]);
          checkRefresh();
        }                
        else if ( data[0].equals("Range")) {  // check for Range header    
          row =  Integer.parseInt(data[1]);
          rangeMin[row] = Integer.parseInt(data[2]);
          rangeMax[row] = Integer.parseInt(data[3]);
          checkRefresh();
        }       
        else if ( data[0].equals("Data")) {  // check for data header            
          row =  Integer.parseInt(data[1]); 
          values[row] = Integer.parseInt(data[2]);
          checkRefresh();
        }
        
      }
    }
    catch (Exception e) {
      e.printStackTrace(); // Display whatever error we received
    }
  } 
  
}  

