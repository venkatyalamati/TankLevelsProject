 #include <EEPROM.h>
 
  ISR(TIMER1_COMPA_vect){ // runs every TIMER1_PERIOD_MILLSEC

    buzzer.timedTurnOff();
    returnHomeScreenTimer.update(); charReceiveMainTimer.update(); charReceiveSubTimer.update(); slaveModeMainLoopTimer.update();

    if(tik_1000ms.tick_Gen_Run()){
      LED_Num_Cir_Cnt.incrCnt();
      if(!tank1.isAlarmInProgress() && !tank2.isAlarmInProgress()){
        if(tank1.beepForMidLvlChanges() || tank2.beepForMidLvlChanges()){
          buzzer.turnOn(beepLenMidLvl);
        }
      }
    }

    tank1AlarmOnOff.executor(tank1_alarm_On_action, tank1_alarm_Off_action, tank1_alarm_fin_action);
    tank2AlarmOnOff.executor(tank2_alarm_On_action, tank2_alarm_Off_action, tank2_alarm_fin_action);

    if(deviceMode == masterMode && currPage == dashBoardPage)
      tik_main_loop_cycle.tick_Gen_Run();

  } // ISR(TIMER1_COMPA_vect) ends

  void tank1_alarm_On_action(){
    buzzer.turnOn(5); shiftOutDOsPort1.enableOutput();
  }
  void tank1_alarm_Off_action(){
    shiftOutDOsPort1.disableOutput();
  }
  void tank1_alarm_fin_action(){
    shiftOutDOsPort1.enableOutput();
    tank1.setAlarmStatusFinished();
  }

  void tank2_alarm_On_action(){
    buzzer.turnOn(5); shiftOutDOsPort2.enableOutput();
  }
  void tank2_alarm_Off_action(){
    shiftOutDOsPort2.disableOutput();
  }
  void tank2_alarm_fin_action(){
    shiftOutDOsPort2.enableOutput();
    tank2.setAlarmStatusFinished();
  }

  ISR(TIMER2_COMPA_vect) { // runs every TIMER2_PERIOD_MILLSEC
    ButtonTimer2Based::timer2_isr_tick++;
    if(currPage == dashBoardPage){
      mode_set_but_pin.scanButton();
    }
    mid_but_pin.scanButton();
    lhs_but_pin.scanButton();
    rhs_but_pin.scanButton();
  } // ISR(TIMER2_COMPA_vect) ends

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

