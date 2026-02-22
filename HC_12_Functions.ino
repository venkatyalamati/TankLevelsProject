void receiveCharLevels_And_Convert(){ //Rx: -28.57-28.57
  uint8_t j=0; char ch='\0';
  // Clear buffer
  for (uint8_t i = 0; i < bufferSize; i++)
    buffer[i] = '\0';
  if(currPage == dashBoardPage)
    charReceiveWaitingTimer.startTimer(MAIN_LOOP_CYCLE_PERIOD+2000);
  j=0;
  while(true){
  // Check for available data from HC-12
    if(Serial.available()){
      if(!charReceiveFinishTimer.isTimerRunning())
        charReceiveFinishTimer.startTimer(1000);
      
      if(j < (bufferSize-1)){
        // Store character if buffer isn't full
        ch = Serial.read();
        buffer[j] = ch;
        j++;
      }
      else{
        for(uint8_t i = 0; i < bufferSize; i++)
          buffer[i] = '\0';
        j = 0;
      }

      // Check for newline to indicate end of message
      if(ch == '\n' && j == (bufferSize-1)){
        for(uint8_t i=0; i<7; i++) levelsPercentageChar[i] = '\0';
        for(uint8_t i=0; i<6; i++) levelsPercentageChar[i] = buffer[i];
        levelsPercentageFloat[0] = floatFromCharArray(levelsPercentageChar);

        for(uint8_t i=0; i<7; i++) levelsPercentageChar[i] = '\0';
        for(uint8_t i=0; i<6; i++) levelsPercentageChar[i] = buffer[i+6];
        levelsPercentageFloat[1] = floatFromCharArray(levelsPercentageChar);

        if(currPage == noSgnlRcvdPage){
          displayHomePage(fullPage);
          LED_Num_Cir_Cnt.disableCnt(); // just to avoid unnecessary use of ISR time
        }
        else{
          charReceiveWaitingTimer.stopTimer();
        }
        charReceiveFinishTimer.stopTimer();
        
        return;
      }
      else if(ch == '\n' && j != (bufferSize-1)){
        for(uint8_t i = 0; i < bufferSize; i++)
          buffer[i] = '\0';
        j = 0;
      }
    
    } // if(Serial.available()) ends

    if(charReceiveFinishTimer.isTimerRunning()){
      if(charReceiveFinishTimer.checkTimeOut()){
        for(uint8_t i = 0; i < bufferSize; i++)
          buffer[i] = '\0';
        j = 0;
        
      }
    }
    else if(charReceiveWaitingTimer.isTimerRunning()){
      if(charReceiveWaitingTimer.checkTimeOut()){
        displaySignalNotRcvd();
      }
    }
    else{
      blinkLEDs_usingTimerISR(); break;
    }
    
  } // infinite while loop
    
} // receiveCharLevels_And_Convert function ends

void convertLevelsToChar_And_Transmit(){
  for (uint8_t i = 0; i < bufferSize; i++)
    buffer[i] = '\0';

  for (uint8_t i = 0; i < 7; i++)
    levelsPercentageChar[i] = '\0';
  
  dtostrf(levelsPercentageFloat[0], 6, 2, levelsPercentageChar);
  for(uint8_t i=0; i<6; i++){
    buffer[i] = levelsPercentageChar[i];
  }

  for (uint8_t i = 0; i < 7; i++)
    levelsPercentageChar[i] = '\0';

  dtostrf(levelsPercentageFloat[1], 6, 2, levelsPercentageChar);
  for(uint8_t i=0; i<6; i++){
    buffer[i+6] = levelsPercentageChar[i];
  }
  
  Serial.println(buffer);
}

void clearSerialBuffer(){
  CalcTimeElapsed localTimer;
  localTimer.startTimer();
  while(true){
    if(Serial.available()){
      Serial.read(); // Discard all existing data
      localTimer.startTimer();
    }
  
    if(localTimer.getTimeElapsedMilSec() > 1000){
      break;
    }
  } // infinite while
}