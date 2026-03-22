void displayHomePage(diplayPages diplayPageType){
int intLevel;

  if(diplayPageType == fullPage){
    tftST7735.clearScreen(); tftST7735.setBigFont(); tftST7735.clearCharBuffer();
    if(deviceMode == masterMode){
      strcpy_P(tftST7735.charBuffer, (PGM_P)F("LEVELS-M")); tftST7735.textPrintBigFont(1);
    }
    else{
      strcpy_P(tftST7735.charBuffer, (PGM_P)F("LEVELS-S")); tftST7735.textPrintBigFont(1);
      clearSerialBuffer(); // fullPage and slave mode
    }
  }
  // ----------- 1st Tank ------------------ 
    strcpy_P(tftST7735.charBuffer, (PGM_P)F("T-1 000%"));
    if(diplayPageType == updtValues){
      intLevel = round(levelsPercentageFloat[0]);
      DecimalToCharFunctions::insertIntVal_intoCharArray(tftST7735.charBuffer, 5, 3, intLevel, false);
    }
    tftST7735.textPrintBigFont(2.30); 
  // ----------- 2nd Tank ------------------
    strcpy_P(tftST7735.charBuffer, (PGM_P)F("T-2 000%"));
    if(diplayPageType == updtValues){
      intLevel = round(levelsPercentageFloat[1]);
      DecimalToCharFunctions::insertIntVal_intoCharArray(tftST7735.charBuffer, 5, 3, intLevel, false);
    }
    tftST7735.textPrintBigFont(3.70); 
//----------------------------------------
    currPage = dashBoardPage;
}

void displaySignalNotRcvd(){
  tftST7735.clearScreen(); tftST7735.setBigFont(); tftST7735.clearCharBuffer();

  // ----------- 1st Line ------------------ 
  strcpy_P(tftST7735.charBuffer, (PGM_P)F("   NO   ")); tftST7735.textPrintBigFont(1.0);
  // ----------- 2nd Line ------------------   
  strcpy_P(tftST7735.charBuffer, (PGM_P)F(" SIGNAL ")); tftST7735.textPrintBigFont(2.30);
  // ----------- 3rd Line ------------------ 
  strcpy_P(tftST7735.charBuffer, (PGM_P)F("RECEIVED")); tftST7735.textPrintBigFont(3.70);
  currPage = noSgnlRcvdPage;
}

void displaySlctTankPage(){
  tftST7735.clearScreen(); tftST7735.setSmallFont(); tftST7735.clearCharBuffer();
  strcpy_P(tftST7735.charBuffer, (PGM_P)F("     Select     ")); tftST7735.textPrintSmallFont(1);
  strcpy_P(tftST7735.charBuffer, (PGM_P)F("    the tank    ")); tftST7735.textPrintSmallFont(2);
  strcpy_P(tftST7735.charBuffer, (PGM_P)F("  to calibrate  ")); tftST7735.textPrintSmallFont(3);
  strcpy_P(tftST7735.charBuffer, (PGM_P)F(" Sensor Output: ")); tftST7735.textPrintSmallFont(4);
  strcpy_P(tftST7735.charBuffer, (PGM_P)F("Tank1      Tank2")); tftST7735.textPrintSmallFont(6);
  //toDispOLED();
  currPage = slctTankPage;
  returnHomeScreenTimer.start_finOneShot(30000);
}

void displayCalibSensorPage(){
  tftST7735.clearScreen(); tftST7735.setSmallFont(); tftST7735.clearCharBuffer();
  strcpy_P(tftST7735.charBuffer, (PGM_P)F("Calibrate Tank-x")); tftST7735.charBuffer[15] = tankSel + 48; tftST7735.textPrintSmallFont(1);
  strcpy_P(tftST7735.charBuffer, (PGM_P)F("   sensor O/P   ")); tftST7735.textPrintSmallFont(2);
  strcpy_P(tftST7735.charBuffer, (PGM_P)F("  Mark present  ")); tftST7735.textPrintSmallFont(3);
  strcpy_P(tftST7735.charBuffer, (PGM_P)F("    level as    ")); tftST7735.textPrintSmallFont(4);
  strcpy_P(tftST7735.charBuffer, (PGM_P)F("EMPTY       FULL")); tftST7735.textPrintSmallFont(6);
  //toDispOLED();
  currPage = calSensorPage;
  returnHomeScreenTimer.start_finOneShot(30000);
}

void displayCalibDone_EmptyLevel(){
  tftST7735.clearScreen(); tftST7735.setSmallFont(); tftST7735.clearCharBuffer();
  strcpy_P(tftST7735.charBuffer, (PGM_P)F("Calibration Done")); tftST7735.textPrintSmallFont(1);
  strcpy_P(tftST7735.charBuffer, (PGM_P)F("   for Tank-x   ")); tftST7735.charBuffer[12] = tankSel + 48; tftST7735.textPrintSmallFont(2);
  strcpy_P(tftST7735.charBuffer, (PGM_P)F(" Marked present ")); tftST7735.textPrintSmallFont(3);
  strcpy_P(tftST7735.charBuffer, (PGM_P)F(" tank level as: ")); tftST7735.textPrintSmallFont(4);
  strcpy_P(tftST7735.charBuffer, (PGM_P)F("   EMPTY (0%)   ")); tftST7735.textPrintSmallFont(6);
  //toDispOLED();
  currPage = showMessagePage;
  returnHomeScreenTimer.start_finOneShot(8000);
}

void displayCalibDone_FullLevel(){
  tftST7735.clearScreen(); tftST7735.setSmallFont(); tftST7735.clearCharBuffer();
  strcpy_P(tftST7735.charBuffer, (PGM_P)F("Calibration Done")); tftST7735.textPrintSmallFont(1);
  strcpy_P(tftST7735.charBuffer, (PGM_P)F("   for Tank-x   ")); tftST7735.charBuffer[12] = tankSel + 48; tftST7735.textPrintSmallFont(2);
  strcpy_P(tftST7735.charBuffer, (PGM_P)F(" Marked present ")); tftST7735.textPrintSmallFont(3);
  strcpy_P(tftST7735.charBuffer, (PGM_P)F(" tank level as: ")); tftST7735.textPrintSmallFont(4);
  strcpy_P(tftST7735.charBuffer, (PGM_P)F("   FULL (100%)  ")); tftST7735.textPrintSmallFont(6);
  //toDispOLED();
  currPage = showMessagePage;
  returnHomeScreenTimer.start_finOneShot(8000);
}

void displaySetNumBeeps(byte diplayPageType){
  char str[3]; byte temp; int i;
  temp = numBeepsOnAlarm;
  for(int16_t i=2; i>=0; i--){
    str[i] = temp % 10; temp /= 10;
    str[i] += 48; // To ASCII
  }
  if(diplayPageType == fullPage){
    tftST7735.clearScreen(); tftST7735.setSmallFont(); tftST7735.clearCharBuffer();
    strcpy_P(tftST7735.charBuffer, (PGM_P)F(" Set the Number ")); tftST7735.textPrintSmallFont(1);
    strcpy_P(tftST7735.charBuffer, (PGM_P)F("  of Beeps for  ")); tftST7735.textPrintSmallFont(2);
    strcpy_P(tftST7735.charBuffer, (PGM_P)F(" Low/High Level ")); tftST7735.textPrintSmallFont(3);
  }
    strcpy_P(tftST7735.charBuffer, (PGM_P)F("   Beeps: 000   ")); 
  for(uint8_t i=0; i<3; i++)
    tftST7735.charBuffer[i+10] = str[i]; 
  tftST7735.textPrintSmallFont(4);
  if(diplayPageType == fullPage){
    strcpy_P(tftST7735.charBuffer, (PGM_P)F("Dec    OK    Inc")); tftST7735.textPrintSmallFont(6);
  }
  //toDispOLED();
  currPage = setNumBeepsPage;
  returnHomeScreenTimer.start_finOneShot(20000);
}

void displayNumBeepsSetDone(){
  char str[3]; byte temp;
  temp = numBeepsOnAlarm;
  for(int16_t i=2; i>=0; i--){
    str[i] = temp % 10; temp /= 10;
    str[i] += 48; // To ASCII
  }
  tftST7735.clearScreen(); tftST7735.setSmallFont(); tftST7735.clearCharBuffer();
  strcpy_P(tftST7735.charBuffer, (PGM_P)F("  No. of Beeps  ")); tftST7735.textPrintSmallFont(1);
  strcpy_P(tftST7735.charBuffer, (PGM_P)F("       for      ")); tftST7735.textPrintSmallFont(2);
  strcpy_P(tftST7735.charBuffer, (PGM_P)F(" Low/High Level ")); tftST7735.textPrintSmallFont(3);
  strcpy_P(tftST7735.charBuffer, (PGM_P)F("     set to     ")); tftST7735.textPrintSmallFont(4);
  strcpy_P(tftST7735.charBuffer, (PGM_P)F("    > '000'     ")); 
  for(uint8_t i=0; i<3; i++)
    tftST7735.charBuffer[i+7] = str[i];
  tftST7735.textPrintSmallFont(5);
  strcpy_P(tftST7735.charBuffer, (PGM_P)F("<<<<<<<  >>>>>>>")); tftST7735.textPrintSmallFont(6);
  currPage = showMessagePage;
  returnHomeScreenTimer.start_finOneShot(8000);
}

void displaySetBeepLenMidLvls(byte diplayPageType){
  char str[4]; int temp;
  temp = beepLenMidLvl * TIMER1_PERIOD_MILLIS;
  for(int16_t i=3; i>=0; i--){
    str[i] = temp % 10; temp /= 10;
    str[i] += 48; // To ASCII
  }
  if(diplayPageType == fullPage){
    tftST7735.clearScreen(); tftST7735.setSmallFont(); tftST7735.clearCharBuffer();
    strcpy_P(tftST7735.charBuffer, (PGM_P)F("  Set the beep  ")); tftST7735.textPrintSmallFont(1);
    strcpy_P(tftST7735.charBuffer, (PGM_P)F(" length for mid ")); tftST7735.textPrintSmallFont(2);
    strcpy_P(tftST7735.charBuffer, (PGM_P)F(" levels changes ")); tftST7735.textPrintSmallFont(3);
    strcpy_P(tftST7735.charBuffer, (PGM_P)F("(in milli sec's)")); tftST7735.textPrintSmallFont(4);
  }
    strcpy_P(tftST7735.charBuffer, (PGM_P)F("      0000      ")); 
  for(uint8_t i=0; i<4; i++)
    tftST7735.charBuffer[i+6] = str[i]; 
  tftST7735.textPrintSmallFont(5);
  if(diplayPageType == fullPage){
    strcpy_P(tftST7735.charBuffer, (PGM_P)F("Dec    OK    Inc")); tftST7735.textPrintSmallFont(6);
  }
  //toDispOLED();
  currPage = setMidLvlBeepLenPage;
  returnHomeScreenTimer.start_finOneShot(20000);
}

void displayBeepLenMidLvlsSetDone(){
  char str[4]; int temp;
  temp = beepLenMidLvl * TIMER1_PERIOD_MILLIS;
  for(int16_t i=3; i>=0; i--){
    str[i] = temp % 10; temp /= 10;
    str[i] += 48; // To ASCII
  }
  tftST7735.clearScreen(); tftST7735.setSmallFont(); tftST7735.clearCharBuffer();
  strcpy_P(tftST7735.charBuffer, (PGM_P)F("  Beep length:  ")); tftST7735.textPrintSmallFont(1);
  strcpy_P(tftST7735.charBuffer, (PGM_P)F(" for mid levels ")); tftST7735.textPrintSmallFont(2);
  strcpy_P(tftST7735.charBuffer, (PGM_P)F("     set to     ")); tftST7735.textPrintSmallFont(3);
  strcpy_P(tftST7735.charBuffer, (PGM_P)F("    > 0000      ")); 
  for(uint8_t i=0; i<4; i++)
    tftST7735.charBuffer[i+6] = str[i];
  tftST7735.textPrintSmallFont(4);
  strcpy_P(tftST7735.charBuffer, (PGM_P)F("   milli sec's  ")); tftST7735.textPrintSmallFont(5);
  strcpy_P(tftST7735.charBuffer, (PGM_P)F("<<<<<<<  >>>>>>>")); tftST7735.textPrintSmallFont(6);
  currPage = showMessagePage;
  returnHomeScreenTimer.start_finOneShot(8000);
}

void displaySetDeviceModePage(byte diplayPageType){
  if(diplayPageType == fullPage){
    tftST7735.clearScreen(); tftST7735.setSmallFont(); tftST7735.clearCharBuffer();
    strcpy_P(tftST7735.charBuffer, (PGM_P)F(" Set the Device ")); tftST7735.textPrintSmallFont(1);
    strcpy_P(tftST7735.charBuffer, (PGM_P)F("    Mode To:    ")); tftST7735.textPrintSmallFont(2);
    strcpy_P(tftST7735.charBuffer, (PGM_P)F("  Master/Slave  ")); tftST7735.textPrintSmallFont(3);
  }
  if(deviceMode == masterMode){
    strcpy_P(tftST7735.charBuffer, (PGM_P)F(" Slctd: MST-'M' ")); tftST7735.textPrintSmallFont(4);
  }
  else{
    strcpy_P(tftST7735.charBuffer, (PGM_P)F(" Slctd: SLV-'S' ")); tftST7735.textPrintSmallFont(4);
  }
  if(diplayPageType == fullPage){
    strcpy_P(tftST7735.charBuffer, (PGM_P)F("M      OK      S")); tftST7735.textPrintSmallFont(6);
  }
  //toDispOLED();
  currPage = setDeviceModePage;
  returnHomeScreenTimer.start_finOneShot(8000);
}

void displayDeviceModeSetDone(){
  tftST7735.clearScreen(); tftST7735.setSmallFont(); tftST7735.clearCharBuffer();
  strcpy_P(tftST7735.charBuffer, (PGM_P)F("   The Device   ")); tftST7735.textPrintSmallFont(1);
  strcpy_P(tftST7735.charBuffer, (PGM_P)F("Operating   Mode")); tftST7735.textPrintSmallFont(2);
  strcpy_P(tftST7735.charBuffer, (PGM_P)F("   is set to    ")); tftST7735.textPrintSmallFont(3);

  if(deviceMode == masterMode){
  strcpy_P(tftST7735.charBuffer, (PGM_P)F("   > MST -'M'   ")); tftST7735.textPrintSmallFont(5);
  }
  else{
  strcpy_P(tftST7735.charBuffer, (PGM_P)F("   > SLV -'S'   ")); tftST7735.textPrintSmallFont(5);
  }
  strcpy_P(tftST7735.charBuffer, (PGM_P)F("<<<<<<<  >>>>>>>")); tftST7735.textPrintSmallFont(6);

  currPage = showMessagePage;
  returnHomeScreenTimer.start_finOneShot(8000);
}