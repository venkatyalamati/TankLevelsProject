#include "CustomClasses.h"
// Note:- Do not use delay(), Serial.print(), setPinMode() etc. inside the constructor
  
  // Tank class implementations
  Tank::Tank(int16_t eepromStartAddr){
    _eepromStartAddr = eepromStartAddr;
    _startAlarmOrder = false;
    _stopAlarmOrder = false;
    _beepForMidLvlChanges = false;
  }
  void Tank::doStartUpActions(){
    for(uint8_t i=0; i<8; i++)
      ledStatus[i] = 0;
    _prevLevelStatus = update_ledStatus_getLevelStatus();
    _prevAlarmStatus = EEPROM.read(_eepromStartAddr);
  }
  void Tank::actionsOnLevel(float levelPercentage){ // -------------- ACTION BASED ON LEVELS RECEIVED FROM SENSORS -------------
    _levelPercentage = levelPercentage;
    if(_prevAlarmStatus != alarmNotReq)
      ledStatus[0] = 0;
    // incase of low level alarm was started, previously made ledStatus[0]=1 (for blinking purpose) leads to _levelStatus as '1'
    
    _levelStatus = update_ledStatus_getLevelStatus();

    if(_levelStatus == 8 || _levelStatus == 0){ // full or empty level
      if(_prevAlarmStatus == alarmFinished){
        _alarmStatus = _prevAlarmStatus;
      }
      else if(_prevAlarmStatus == alarmStarted){
        _alarmStatus = _prevAlarmStatus;
      }
      else{
        _startAlarmOrder = true;
        _alarmStatus = alarmStarted;
      }
    }
    else{ // middle levels
      if(_prevAlarmStatus == alarmStarted){
        _stopAlarmOrder = true;
      }
      _alarmStatus = alarmNotReq;
      if(_prevLevelStatus != _levelStatus)
        _beepForMidLvlChanges = true;
    }

    if(_alarmStatus != _prevAlarmStatus && _alarmStatus != alarmStarted){
      EEPROM.put(_eepromStartAddr, _alarmStatus);
    }

    if(_alarmStatus == alarmStarted){
      ledStatus[0] = 1; // if low level alarm started, this must be done for blinking the bottom led
    }
    _prevLevelStatus = _levelStatus;
    _prevAlarmStatus = _alarmStatus;
    
  } // actionsOnLevel ends

  uint8_t Tank::update_ledStatus_getLevelStatus(){
    uint8_t countHigh = 0;
    for(uint8_t i=0; i<8; i++){   
      if(_levelPercentage >= (i+1)*12.5){
        ledStatus[i] = 1;
      }
      else if(_levelPercentage <= i*12.5){
        ledStatus[i] = 0;
      }
    }
    for(uint8_t i=0; i<8; i++){
      if(ledStatus[i] == 1)
        countHigh++;
    }
    return countHigh;
  }
  bool Tank::getStartAlarmOrder(){
    if(_startAlarmOrder){
      _startAlarmOrder = false;
      return true;
    }
    else{
      return false;
    }
  }
  bool Tank::getStopAlarmOrder(){
    if(_stopAlarmOrder){
      _stopAlarmOrder = false;
      return true;
    }
    else{
      return false;
    }
  }
  bool Tank::isAlarmInProgress(){
    if(_prevAlarmStatus == alarmStarted)
      return true;
    else
      return false;
  }
  bool Tank::beepForMidLvlChanges(){
    if(_beepForMidLvlChanges){
      _beepForMidLvlChanges = false;
      return true;
    }
    else{
      return false;
    }
  }
  void Tank::setAlarmStatusFinished(){
    _prevAlarmStatus = alarmFinished;
  }

