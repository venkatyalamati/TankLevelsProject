void displayHomePage(diplayPages diplayPageType){
int intLevel;

  if(diplayPageType == fullPage){
    ucg.clearScreen();
    ucg.setFont(ucg_font_inr21_mr); // " " will overwrite
    if(deviceMode == masterMode){
      strcpy_P(arr8, (PGM_P)F("LEVELS-M")); textPrintOnTFT_BigFont(arr8, 1.0);
    }
    else{
      strcpy_P(arr8, (PGM_P)F("LEVELS-S")); textPrintOnTFT_BigFont(arr8, 1.0);
    }
  }
  // ----------- 1st Tank ------------------ 
    strcpy_P(arr8, (PGM_P)F("T-1 000%"));
    if(diplayPageType == updtValues){
      intLevel = round(levelsPercentageFloat[0]); //intLevel = static_cast<int>(levelsPercentage[0]);
      convAndStoreToArr8(intLevel, 5);
    }
    textPrintOnTFT_BigFont(arr8, 2.30); 
  // ----------- 2nd Tank ------------------
    strcpy_P(arr8, (PGM_P)F("T-2 000%"));
    if(diplayPageType == updtValues){
      intLevel = round(levelsPercentageFloat[1]);
      convAndStoreToArr8(intLevel, 5);
    }
    textPrintOnTFT_BigFont(arr8, 3.70); 
//----------------------------------------
    currPage = dashBoardPage;

}

void displaySignalNotRcvd(){
  ucg.clearScreen();
  ucg.setFont(ucg_font_inr21_mr); // " " will overwrite

  // ----------- 1st Line ------------------ 
  strcpy_P(arr8, (PGM_P)F("   NO   ")); textPrintOnTFT_BigFont(arr8, 1.0);
  // ----------- 2nd Line ------------------   
  strcpy_P(arr8, (PGM_P)F(" SIGNAL ")); textPrintOnTFT_BigFont(arr8, 2.30);
  // ----------- 3rd Line ------------------ 
  strcpy_P(arr8, (PGM_P)F("RECEIVED")); textPrintOnTFT_BigFont(arr8, 3.70);
  currPage = noSgnlRcvdPage;
}

void displaySlctTankPage(){
  ucg.clearScreen();
  ucg.setFont(ucg_font_9x18_tf);
  strcpy_P(arr16, (PGM_P)F("     Select     ")); textPrintOnTFT_SmallFont(arr16, 1);
  strcpy_P(arr16, (PGM_P)F("    the tank    ")); textPrintOnTFT_SmallFont(arr16, 2);
  strcpy_P(arr16, (PGM_P)F("  to calibrate  ")); textPrintOnTFT_SmallFont(arr16, 3);
  strcpy_P(arr16, (PGM_P)F(" Sensor Output: ")); textPrintOnTFT_SmallFont(arr16, 4);
  strcpy_P(arr16, (PGM_P)F("Tank1      Tank2")); textPrintOnTFT_SmallFont(arr16, 6);
  //toDispOLED();
  currPage = slctTankPage;
  returnHomeScrOnTimeOut.startTimer(30000);
}

void displayCalibSensorPage(){
  ucg.clearScreen();
  ucg.setFont(ucg_font_9x18_tf);
  strcpy_P(arr16, (PGM_P)F("Calibrate Tank-x")); arr16[15] = tankSel + 48; textPrintOnTFT_SmallFont(arr16, 1);
  strcpy_P(arr16, (PGM_P)F("   sensor O/P   ")); textPrintOnTFT_SmallFont(arr16, 2);
  strcpy_P(arr16, (PGM_P)F("  Mark present  ")); textPrintOnTFT_SmallFont(arr16, 3);
  strcpy_P(arr16, (PGM_P)F("    level as    ")); textPrintOnTFT_SmallFont(arr16, 4);
  strcpy_P(arr16, (PGM_P)F("EMPTY       FULL")); textPrintOnTFT_SmallFont(arr16, 6);
  //toDispOLED();
  currPage = calSensorPage;
  returnHomeScrOnTimeOut.startTimer(30000);
}

void displayCalibDone_EmptyLevel(){
  ucg.clearScreen();
  ucg.setFont(ucg_font_9x18_tf);
  strcpy_P(arr16, (PGM_P)F("Calibration Done")); textPrintOnTFT_SmallFont(arr16, 1);
  strcpy_P(arr16, (PGM_P)F("   for Tank-x   ")); arr16[12] = tankSel + 48; textPrintOnTFT_SmallFont(arr16, 2);
  strcpy_P(arr16, (PGM_P)F(" Marked present ")); textPrintOnTFT_SmallFont(arr16, 3);
  strcpy_P(arr16, (PGM_P)F(" tank level as: ")); textPrintOnTFT_SmallFont(arr16, 4);
  strcpy_P(arr16, (PGM_P)F("   EMPTY (0%)   ")); textPrintOnTFT_SmallFont(arr16, 6);
  //toDispOLED();
  currPage = showMessagePage;
  returnHomeScrOnTimeOut.startTimer(8000);
}

void displayCalibDone_FullLevel(){
  ucg.clearScreen();
  ucg.setFont(ucg_font_9x18_tf);
  strcpy_P(arr16, (PGM_P)F("Calibration Done")); textPrintOnTFT_SmallFont(arr16, 1);
  strcpy_P(arr16, (PGM_P)F("   for Tank-x   ")); arr16[12] = tankSel + 48; textPrintOnTFT_SmallFont(arr16, 2);
  strcpy_P(arr16, (PGM_P)F(" Marked present ")); textPrintOnTFT_SmallFont(arr16, 3);
  strcpy_P(arr16, (PGM_P)F(" tank level as: ")); textPrintOnTFT_SmallFont(arr16, 4);
  strcpy_P(arr16, (PGM_P)F("   FULL (100%)  ")); textPrintOnTFT_SmallFont(arr16, 6);
  //toDispOLED();
  currPage = showMessagePage;
  returnHomeScrOnTimeOut.startTimer(8000);
}

void displaySetNumBeeps(byte diplayPageType){
  char str[3]; byte temp; int i;
  temp = numBeepsOnAlarm;
  for(i=2; i>=0; i--){
    str[i] = temp % 10; temp /= 10;
    str[i] += 48; // To ASCII
  }
  if(diplayPageType == fullPage){
    ucg.clearScreen();
    ucg.setFont(ucg_font_9x18_tf);
    strcpy_P(arr16, (PGM_P)F(" Set the Number ")); textPrintOnTFT_SmallFont(arr16, 1);
    strcpy_P(arr16, (PGM_P)F("  of Beeps for  ")); textPrintOnTFT_SmallFont(arr16, 2);
    strcpy_P(arr16, (PGM_P)F(" Low/High Level ")); textPrintOnTFT_SmallFont(arr16, 3);
  }
    strcpy_P(arr16, (PGM_P)F("   Beeps: 000   ")); 
  for(i=0; i<3; i++)
    arr16[i+10] = str[i]; 
  textPrintOnTFT_SmallFont(arr16, 4);
  if(diplayPageType == fullPage){
    strcpy_P(arr16, (PGM_P)F("Dec    OK    Inc")); textPrintOnTFT_SmallFont(arr16, 6);
  }
  //toDispOLED();
  currPage = setNumBeepsPage;
  returnHomeScrOnTimeOut.startTimer(20000);
}

void displayNumBeepsSetDone(){
  char str[3]; byte temp; int i;
  temp = numBeepsOnAlarm;
  for(i=2; i>=0; i--){
    str[i] = temp % 10; temp /= 10;
    str[i] += 48; // To ASCII
  }
  ucg.clearScreen();
  ucg.setFont(ucg_font_9x18_tf);
  strcpy_P(arr16, (PGM_P)F("  No. of Beeps  ")); textPrintOnTFT_SmallFont(arr16, 1);
  strcpy_P(arr16, (PGM_P)F("       for      ")); textPrintOnTFT_SmallFont(arr16, 2);
  strcpy_P(arr16, (PGM_P)F(" Low/High Level ")); textPrintOnTFT_SmallFont(arr16, 3);
  strcpy_P(arr16, (PGM_P)F("     set to     ")); textPrintOnTFT_SmallFont(arr16, 4);
  strcpy_P(arr16, (PGM_P)F("    > '000'     ")); 
  for(i=0; i<3; i++)
    arr16[i+7] = str[i];
  textPrintOnTFT_SmallFont(arr16, 5);
  strcpy_P(arr16, (PGM_P)F("<<<<<<<  >>>>>>>")); textPrintOnTFT_SmallFont(arr16, 6);
  currPage = showMessagePage;
  returnHomeScrOnTimeOut.startTimer(8000);
}

void displaySetBeepLenMidLvls(byte diplayPageType){
  char str[4]; int temp; int i;
  temp = beepLenMidLvl * TIMER1_PERIOD_MILLSEC;
  for(i=3; i>=0; i--){
    str[i] = temp % 10; temp /= 10;
    str[i] += 48; // To ASCII
  }
  if(diplayPageType == fullPage){
    ucg.clearScreen();
    ucg.setFont(ucg_font_9x18_tf);
    strcpy_P(arr16, (PGM_P)F("  Set the beep  ")); textPrintOnTFT_SmallFont(arr16, 1);
    strcpy_P(arr16, (PGM_P)F(" length for mid ")); textPrintOnTFT_SmallFont(arr16, 2);
    strcpy_P(arr16, (PGM_P)F(" levels changes ")); textPrintOnTFT_SmallFont(arr16, 3);
    strcpy_P(arr16, (PGM_P)F("(in milli sec's)")); textPrintOnTFT_SmallFont(arr16, 4);
  }
    strcpy_P(arr16, (PGM_P)F("      0000      ")); 
  for(i=0; i<4; i++)
    arr16[i+6] = str[i]; 
  textPrintOnTFT_SmallFont(arr16, 5);
  if(diplayPageType == fullPage){
    strcpy_P(arr16, (PGM_P)F("Dec    OK    Inc")); textPrintOnTFT_SmallFont(arr16, 6);
  }
  //toDispOLED();
  currPage = setMidLvlBeepLenPage;
  returnHomeScrOnTimeOut.startTimer(20000);
}

void displayBeepLenMidLvlsSetDone(){
  char str[4]; int temp; int i;
  temp = beepLenMidLvl * TIMER1_PERIOD_MILLSEC;
  for(i=3; i>=0; i--){
    str[i] = temp % 10; temp /= 10;
    str[i] += 48; // To ASCII
  }
  ucg.clearScreen();
  ucg.setFont(ucg_font_9x18_tf);
  strcpy_P(arr16, (PGM_P)F("  Beep length:  ")); textPrintOnTFT_SmallFont(arr16, 1);
  strcpy_P(arr16, (PGM_P)F(" for mid levels ")); textPrintOnTFT_SmallFont(arr16, 2);
  strcpy_P(arr16, (PGM_P)F("     set to     ")); textPrintOnTFT_SmallFont(arr16, 3);
  strcpy_P(arr16, (PGM_P)F("    > 0000      ")); 
  for(i=0; i<4; i++)
    arr16[i+6] = str[i];
  textPrintOnTFT_SmallFont(arr16, 4);
  strcpy_P(arr16, (PGM_P)F("   milli sec's  ")); textPrintOnTFT_SmallFont(arr16, 5);
  strcpy_P(arr16, (PGM_P)F("<<<<<<<  >>>>>>>")); textPrintOnTFT_SmallFont(arr16, 6);
  currPage = showMessagePage;
  returnHomeScrOnTimeOut.startTimer(8000);
}

void displaySetDeviceModePage(byte diplayPageType){
  if(diplayPageType == fullPage){
    ucg.clearScreen();
    ucg.setFont(ucg_font_9x18_tf);
    strcpy_P(arr16, (PGM_P)F(" Set the Device ")); textPrintOnTFT_SmallFont(arr16, 1);
    strcpy_P(arr16, (PGM_P)F("    Mode To:    ")); textPrintOnTFT_SmallFont(arr16, 2);
    strcpy_P(arr16, (PGM_P)F("  Master/Slave  ")); textPrintOnTFT_SmallFont(arr16, 3);
  }
  if(deviceMode == masterMode){
    strcpy_P(arr16, (PGM_P)F(" Slctd: MST-'M' ")); textPrintOnTFT_SmallFont(arr16, 4);
  }
  else{
    strcpy_P(arr16, (PGM_P)F(" Slctd: SLV-'S' ")); textPrintOnTFT_SmallFont(arr16, 4);
  }
  if(diplayPageType == fullPage){
    strcpy_P(arr16, (PGM_P)F("M      OK      S")); textPrintOnTFT_SmallFont(arr16, 6);
  }
  //toDispOLED();
  currPage = setDeviceModePage;
  returnHomeScrOnTimeOut.startTimer(8000);
}

void displayDeviceModeSetDone(){
  ucg.clearScreen();
  ucg.setFont(ucg_font_9x18_tf);
  strcpy_P(arr16, (PGM_P)F("   The Device   ")); textPrintOnTFT_SmallFont(arr16, 1);
  strcpy_P(arr16, (PGM_P)F("Operating   Mode")); textPrintOnTFT_SmallFont(arr16, 2);
  strcpy_P(arr16, (PGM_P)F("   is set to    ")); textPrintOnTFT_SmallFont(arr16, 3);

  if(deviceMode == masterMode){
  strcpy_P(arr16, (PGM_P)F("   > MST -'M'   ")); textPrintOnTFT_SmallFont(arr16, 5);
  }
  else{
  strcpy_P(arr16, (PGM_P)F("   > SLV -'S'   ")); textPrintOnTFT_SmallFont(arr16, 5);
  }
  strcpy_P(arr16, (PGM_P)F("<<<<<<<  >>>>>>>")); textPrintOnTFT_SmallFont(arr16, 6);

  currPage = showMessagePage;
  returnHomeScrOnTimeOut.startTimer(8000);
}

void textPrintOnTFT_SmallFont(char txtLine[], byte linePos){ // 13 char max, linPos 1,2,3..
  ucg.setPrintPos(9, linePos*20);
  ucg.print(txtLine);
}

void textPrintOnTFT_BigFont(char txtLine[], float linePos){ // 13 char max, linPos 1,2,3..
  ucg.setPrintPos(9, linePos*30);
  ucg.print(txtLine);
}

void convAndStoreToArr8(int level, byte charStartPos){
char str[3]; int i; bool levelIsNegative;

  if(level >= 0){
    levelIsNegative = false;
  }
  else{
    levelIsNegative = true;
    level = (-1)*level;
    if(level > 99)
      level = 99;
  }

  for(i=2; i>=0; i--){
    str[i] = level % 10; level /= 10;
    str[i] += 48; // To ASCII
  }

  if(levelIsNegative)
    str[0] = '-';

  for(i=0; i<3; i++)
    arr8[i+charStartPos-1] = str[i];
}

// void toDispOLED(){
//   u8g2.firstPage();
//   do {  
//      u8g2.drawStr( 0, 15, dispScrLines[0]); // each line 16 characters max
//      u8g2.drawStr( 0, 30, dispScrLines[1]);
//      u8g2.drawStr( 0, 45, dispScrLines[2]);
//      u8g2.drawStr( 0, 60, dispScrLines[3]); // total 4 lines max
//   }while(u8g2.nextPage());
// }