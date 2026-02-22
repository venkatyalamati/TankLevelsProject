 #include <EEPROM.h>
 
  ISR(TIMER1_COMPA_vect){ // runs after every Timer1Period

    buzzer.timedTurnOff(); returnHomeScrOnTimeOut.runTimer();
    charReceiveWaitingTimer.runTimer(); charReceiveFinishTimer.runTimer();

    if(tik_800ms.tick_Gen_Run()){ //tik_ms tasks must be kept in increasing order of milli sec's
      if(tank1.ledBlinkAct.switchOff())
        shiftOutDOsPort1.disableOutput();

      if(tank2.ledBlinkAct.switchOff())
        shiftOutDOsPort2.disableOutput();

      if(!tank1.alarmInProgress && !tank2.alarmInProgress){
        if(tank1.beepForMidLevels || tank2.beepForMidLevels){
          tank1.beepForMidLevels = false; tank2.beepForMidLevels = false;
          buzzer.turnOn(beepLenMidLvl);
        }
      }
    }

    if(tik_1000ms.tick_Gen_Run()){
      LED_Num_Cir_Cnt.incrCnt();
    }

    if(tik_1600ms.tick_Gen_Run()){
      if(tank1.ledBlinkAct.switchOn()||tank2.ledBlinkAct.switchOn()){
        buzzer.turnOn(5);
      }
      shiftOutDOsPort1.enableOutput();
      shiftOutDOsPort2.enableOutput();
    }

    if(deviceMode == masterMode && currPage == dashBoardPage)
      tik_main_loop_cycle.tick_Gen_Run();

  } // ISR ends

  // -------------- EEPROM related functions ---------------
  
  void storeDfltBeepSettings(){
    EEPROM.write(EEPROM_ADDR_NUM_BEEPS_ON_ALARM, 10); delay(10);
    EEPROM.write(EEPROM_ADDR_BEEP_LEN_MID_LVL, 5); delay(10);
  }

  void loadBeepSettings(){
    numBeepsOnAlarm = EEPROM.read(EEPROM_ADDR_NUM_BEEPS_ON_ALARM);
    beepLenMidLvl = EEPROM.read(EEPROM_ADDR_BEEP_LEN_MID_LVL);
  }

  void storeDfltDeviceModeSetting(){
    EEPROM.write(EEPROM_ADDR_DEVICE_MODE, 'M'); delay(10);
  }

  void loadDeviceModeSetting(){
    char tempVar;
    tempVar = EEPROM.read(EEPROM_ADDR_DEVICE_MODE);
    if(tempVar == 'M'){
      deviceMode = masterMode;
    }
    else{
      deviceMode = slaveMode;
    }
  }

