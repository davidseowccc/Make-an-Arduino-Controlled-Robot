
void simulateData()
{
  labelList.set(1, "Left Line");
  labelList.set(2, "Right Line");
  labelList.set(3, "Sensitivity");
  labelList.set(4, "test");
  //leftMargin = getleftMarginLen() ; 
  adjustSize();
  rangeMin[1] = -127;
  rangeMin[2] = -100;
  rangeMin[3] = 0;
  rangeMax[1] = 127;
  rangeMax[2] = 100;
  rangeMax[3] = 1023;
  values[1] = -126;
  values[2] = 100;
  values[3] = 512;
  values[4] = 1023;
  adjustSize();
}

