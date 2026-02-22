void actionOnButtonPress(byte pinNum){
  switch(pinNum) {
    case MID_BUT_PIN:
      if(currPage == dashBoardPage){
        if(deviceMode == masterMode){
          displaySlctTankPage();
        }
      }
      else if(currPage == setNumBeepsPage){
        displayNumBeepsSetDone();
        EEPROM.update(EEPROM_ADDR_NUM_BEEPS_ON_ALARM, numBeepsOnAlarm); delay(10);
      }
      else if(currPage == setMidLvlBeepLenPage){
        displayBeepLenMidLvlsSetDone();
        EEPROM.update(EEPROM_ADDR_BEEP_LEN_MID_LVL, beepLenMidLvl); delay(10);
      }
      else if(currPage == setDeviceModePage){
        displayDeviceModeSetDone();
        if(deviceMode == masterMode)
          EEPROM.write(EEPROM_ADDR_DEVICE_MODE, 'M');
        else if(deviceMode == slaveMode)
          EEPROM.write(EEPROM_ADDR_DEVICE_MODE, 'S');
        delay(10);
      }
      else{
        returnHomeScrOnTimeOut.forceTimeOut();
      }
      break;
    case LHS_BUT_PIN:
      if(currPage == dashBoardPage){
        displaySetNumBeeps(fullPage);
      }
      else if(currPage == slctTankPage && deviceMode == masterMode){
        tankSel = 1; // selected Tank1
        displayCalibSensorPage();
      }
      else if(currPage == calSensorPage && deviceMode == masterMode){
        // execute calibration(may be tank1 or tank2) for low level
        displayCalibDone_EmptyLevel();
        if(tankSel == 1){
          levelSensor1.calibrateEmpty();
        }
        else if(tankSel == 2){
          levelSensor2.calibrateEmpty();
        }
        else{}
      }
      else if(currPage == setNumBeepsPage){
        if(numBeepsOnAlarm > 5)
          numBeepsOnAlarm -= 5;
        displaySetNumBeeps(updtValues);
      }
      else if(currPage == setMidLvlBeepLenPage){
        if(beepLenMidLvl > 0)
          beepLenMidLvl--;
        displaySetBeepLenMidLvls(updtValues);
      }
      else if(currPage == setDeviceModePage){
        deviceMode = masterMode;
        displaySetDeviceModePage(updtValues);
      }
      else{}
      break;
    case RHS_BUT_PIN:
      if(currPage == dashBoardPage){
        displaySetBeepLenMidLvls(fullPage);
      }
      else if(currPage==slctTankPage && deviceMode == masterMode){
        tankSel = 2; // selected Tank2
        displayCalibSensorPage();
      }
      else if(currPage==calSensorPage && deviceMode == masterMode){
        // execute calibration(may be tank1 or tank2) for high level
        displayCalibDone_FullLevel();
        if(tankSel == 1){
          levelSensor1.calibrateFull();
        }
        else if(tankSel == 2){
          levelSensor2.calibrateFull();
        }
        else{}
      }
      else if(currPage == setNumBeepsPage){
        if(numBeepsOnAlarm < 250)
          numBeepsOnAlarm += 5;
        displaySetNumBeeps(updtValues);
      }
      else if(currPage == setMidLvlBeepLenPage){
        if(beepLenMidLvl < 10)
          beepLenMidLvl++;
        displaySetBeepLenMidLvls(updtValues);
      }
      else if(currPage == setDeviceModePage){
        deviceMode = slaveMode;
        displaySetDeviceModePage(updtValues);
      }
      else{}
      break;
    case MODE_SET_BUT_PIN:
      if(currPage == dashBoardPage){
        displaySetDeviceModePage(fullPage);
      }
      else{}
      break;
    default:
      // code to be executed if none of the above cases are true
      break;
    }
}

void readButton(byte pinNum){
  if(digitalRead(pinNum)==LOW){
    byte i = 0;
    //wdt_reset();
    for(i=0; i<10; i++){ // ensure button pressed for 50 msec
     delay(5);
     if(digitalRead(pinNum)==HIGH){
        return;
     }
    }
    if(i>9){
      // button pressed - call for action
      actionOnButtonPress(pinNum);
    }
    // ensure button released for > specific time
    for(i=0; i<10; i++){
       if(digitalRead(pinNum)==LOW){
          i=0;
       }
       delay(10);
    }
    // button released
  } // main if
}

void readButtonLongPress(byte pinNum){
  if(digitalRead(pinNum)==LOW){
    byte i = 0;
    //wdt_reset();
    for(i=0; i<10; i++){ // ensure button pressed for 5 sec
     delay(500);
     if(digitalRead(pinNum)==HIGH){
        return;
     }
    }
    if(i>9){
      // button pressed - call for action
      actionOnButtonPress(pinNum);
      // actions that are common for all button press events
      clearAll_LEDs();
      
    }
    // ensure button released for > specific time
    for(i=0; i<10; i++){
       if(digitalRead(pinNum)==LOW){
          i=0;
       }
       delay(10);
    }
    // button released
  } // main if
}