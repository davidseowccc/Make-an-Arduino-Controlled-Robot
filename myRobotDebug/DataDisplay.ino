// DataDisplay

void dataDisplayBegin(int nbrItems, char* labels[], int minRange[], int maxRange[] )
{
   for(int i = 1; i < nbrItems; i++) 
   {
     sendLabel(i, labels[i]); 
     sendRange(i,  minRange[i], maxRange[i]);    
   }
}

void sendLabel( int row, char *label)
{
  sendString("Label"); sendValue(row); sendString(label); Serial.println(); 
}

void sendRange( int row, int min, int max)
{
  sendString("Range"); sendValue(row); sendValue(min); sendValue(max); Serial.println();   
}


void sendData(int row, int val)
{
   sendString("Data"); sendValue(row); sendValue(val); Serial.println(); 
}

void sendValue( int value)
{
   Serial.print(value); Serial.print(","); 
}

void sendString(char *string)
{
   Serial.print(string); Serial.print(","); 
}
