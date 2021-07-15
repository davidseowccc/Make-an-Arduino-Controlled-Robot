/*
 * ArduinoDataDisplay
 * based on Arduino Cookbook code from Recipe 4.4 
 * 
 * Displays bar graphs of sensor data sent as CSV from Arduino
 * in all cases, N is the Row to be associated with the given message
 * Labels sent as: "Label,N,the label\n"  // "the label" is used for Row N
 * Range sent as : "Range,N,Min, Max\n"  // Row N has a range from min to max
 *    if Min is negative then the bar grows from the midpoint of Min and Max, 
 *    else the bar grows from Min
 * Data sent as:  "Data,N,val\n"   // val is plotted for row N
 */

short portIndex = 1;  // select the com port, 0 is the first port

int maxNumberOfRows = 12;
int graphWidth      = 600;
int displayWidth    = 1024;
int displayHeight   = 800;

int fontSize = 12;
PFont fontA; 

int windowWidth;  
int windowHeight;

int graphHeight;
int rectCenter;
int rectLeft;
int rectRight;
int topMargin;
int bottomMargin;
int leftMargin = 50;
int rightMargin = 80;

int textHeight;

ArrayList<String> labelList = new ArrayList<String>();
int [] values   = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int [] rangeMin = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int [] rangeMax = { 0, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024};

float lastMsgTime;
float displayRefreshInterval = 20; // min time between screen draws

void setup() { 
  String os=System.getProperty("os.name");
  println(os);  
  initComms(); 
  fontA = createFont("Arial.normal", fontSize);  
  textFont(fontA);
  textHeight = (int)textAscent();
  for (int i = 0; i <= maxNumberOfRows; i++)
    labelList.add(Integer.toString(i));    
  adjustSize();
  drawGrid();
}

void adjustSize()
{
  topMargin = 3 * textHeight;
  bottomMargin = 0; 
  if (displayWidth > 800) {  
    windowWidth = 800;
    windowHeight = topMargin + bottomMargin + yPos(maxNumberOfRows);
    size(windowWidth, windowHeight);
  }
  else {
    windowWidth = displayWidth;
    windowHeight = displayHeight;
  }
  //leftMargin = getleftMarginLen() ; 
  graphHeight = windowHeight - topMargin - bottomMargin;
  rectCenter = leftMargin + graphWidth / 2;
  rectLeft = leftMargin;
  rectRight = leftMargin + graphWidth;
}

void drawGrid() { 
  fill(0); 
  String Title = "Arduino Data" + commsPortString() ;

  int xPos = (int)( rectCenter - textWidth(Title)/2) ; 
  text(Title, xPos, fontSize*2); // Title

  line(rectLeft, topMargin + textHeight, 
       rectLeft, yPos(maxNumberOfRows) + 2);     // left vertical line
       
  line(rectRight, topMargin + textHeight, rectRight, yPos(maxNumberOfRows )+ 2);  // right line
  line(rectCenter, topMargin+textHeight, rectCenter, yPos(maxNumberOfRows) + 2); // center line

  for (int i=1; i <= maxNumberOfRows; i++) {
    fill(0);
    text(labelList.get(i), 2, yPos(i));  // row labels
    fill(150);    
    String rangeCaption = "(" + rangeMin[i] + "~"  + rangeMax[i] + ")";
    text(rangeCaption, rectRight + textWidth("  "), yPos(i)); // range caption
  }
}

int yPos(int index) {
  return topMargin  + ((index) * textHeight * 2);
}

void drawBar(int rowIndex) {
  fill(204);
  if ( rangeMin[rowIndex] < 0) {
    if (values[rowIndex] < 0) {
      int width = int(map(values[rowIndex], 0, rangeMin[rowIndex], 0, graphWidth/2));        
      rect(rectCenter-width, yPos(rowIndex)-fontSize, width, fontSize);
    }
    else {
      int width = int(map(values[rowIndex], 0, rangeMax[rowIndex], 0, graphWidth/2));
      rect(rectCenter, yPos(rowIndex)-fontSize, width, fontSize);
    }
  }
  else {
    int width=int(map(values[rowIndex], rangeMin[rowIndex], rangeMax[rowIndex], 0,graphWidth));
    rect(rectLeft, yPos(rowIndex)-fontSize, width, fontSize);   //draw the value
  }  
  fill(0);
  text(values[rowIndex], 
       rectRight + (int)textWidth(" (-1000~1000)   "), yPos(rowIndex)); // print the value
}

void processMessages() {
  while(true) { 
    String message = commsGetMessage();
    if (message.length() > 0)
    {
      int row = 0;

      String [] data  = message.split(","); // Split the CSV message 
      if ( data[0].equals("Data")) {  // check for data header            
        row =  Integer.parseInt(data[1]); 
        values[row] = Integer.parseInt(data[2]);
        checkRefresh();   
      } 
      else if ( data[0].equals("Label") ) { // check for label header       
        row =  Integer.parseInt(data[1]); 
        labelList.set(row, data[2]);
        if ( (int)textWidth(data[2]) > leftMargin) { 
          leftMargin = (int)(textWidth(data[2]) + textWidth("  ") + 2) ;
          adjustSize();
        }             
        checkRefresh();
      }                
      else if ( data[0].equals("Range")) {  // check for Range header    
        row =  Integer.parseInt(data[1]);
        rangeMin[row] = Integer.parseInt(data[2]);
        rangeMax[row] = Integer.parseInt(data[3]);
        checkRefresh();
      } 
     else
       println(message) ;
    }
    else
      break; // finish processing when the message length is 0
    }
  }

  void checkRefresh()
  {
    if ( lastMsgTime < 1)
      lastMsgTime = millis(); // update the time if it was reset by the last display refresh
  }

void draw() {
  processMessages();
  if ( millis() - lastMsgTime > displayRefreshInterval)
  {
    background(255); 
    drawGrid();             
    for ( int i=1; i <= maxNumberOfRows; i++) 
    {
      drawBar(i);
    }
    lastMsgTime = 0;
  }
}

/******************************
  code for Serial port
*****************************/

import processing.serial.*;

Serial myPort;  // Create object from Serial class

void initComms(){
  String portName = Serial.list()[portIndex];
  println(Serial.list());
  println(" Connecting to -> " + portName) ;
  myPort = new Serial(this, portName, 9600);

}

String  commsPortString() {
  return " (" + Serial.list()[portIndex] + ")"  ; 
}  

String message;

String commsGetMessage() {

  if (myPort.available() > 0) {
    try {
      message = myPort.readStringUntil(10); 
      if (message != null) {
       // print(message); 
        return message;
      }
    }
    catch (Exception e) {
      e.printStackTrace(); // Display whatever error we received
    }
  }
  return "";
}

