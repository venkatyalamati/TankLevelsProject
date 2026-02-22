#include "CustomClasses.h"
// Note:- Do not use delay(), Serial.print() etc. inside the constructor
  
  // Tank class implementations
  Tank::Tank(int eepromStartAddr){
    _eepromStartAddr = eepromStartAddr;
    _doStartUpActions = true;
    beepForMidLevels = false;
    alarmInProgress = false;
  }

  void Tank::actionsOnLevel(float levelPercentage){ // -------------- ACTION BASED ON LEVELS RECEIVED FROM SENSORS -------------
    _levelPercentage = levelPercentage;

    if(_doStartUpActions){
      for(int i=0; i<8; i++)
        ledStatus[i] = 0;
      _prevLevelStatus = update_ledStatus_getLevelStatus();
      _prevAlarmStatus = EEPROM.read(_eepromStartAddr);
      _doStartUpActions = false;
    }

    if(alarmInProgress)
      ledStatus[0] = 0;
    
    _levelStatus = update_ledStatus_getLevelStatus();

    if(_levelStatus == 8){ // full level
      if(_prevAlarmStatus == highAlarmFinished){
        _alarmStatus = _prevAlarmStatus;
      }
      else{
        if(_prevAlarmStatus != highAlarmStarted){
          ledBlinkAct.srtActivity(numBeepsOnAlarm);
          _alarmStatus = highAlarmStarted;
        }
        else if(!ledBlinkAct.enableActivity){
          _alarmStatus = highAlarmFinished;
        }
        else{
          _alarmStatus = _prevAlarmStatus;
        }
      }
    }
    else if(_levelStatus == 0){ // empty level
      if(_prevAlarmStatus == lowAlarmFinished){
        _alarmStatus = _prevAlarmStatus;
      }
      else{
        if(_prevAlarmStatus != lowAlarmStarted)
        {
          ledBlinkAct.srtActivity(numBeepsOnAlarm);
          _alarmStatus = lowAlarmStarted;
        }
        else if(!ledBlinkAct.enableActivity){
          _alarmStatus = lowAlarmFinished;
        }
        else{
          _alarmStatus = _prevAlarmStatus;
        }
      }
    }
    else{
      _alarmStatus = alarmNotReq;
      ledBlinkAct.enableActivity = false;
      if(_prevLevelStatus != _levelStatus)
        beepForMidLevels = true;
    }
    _prevLevelStatus = _levelStatus;

    if(_alarmStatus != _prevAlarmStatus){
      EEPROM.put(_eepromStartAddr, _alarmStatus);
      delay(10);
    }
    if(_alarmStatus == lowAlarmStarted || _alarmStatus == highAlarmStarted){
      alarmInProgress = true;
      ledStatus[0] = 1;
    }
    else{
      alarmInProgress = false;
    }
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

// LevelSensor class implementations
  LevelSensor::LevelSensor(int sensorPin, int addrEEPROM){
    addrEmptyVal = addrEEPROM;
    addrFullVal = addrEEPROM + 2;
    this->sensorPin = sensorPin;
  }
  void LevelSensor::storeDfltCalParameters(){
    EEPROM.put(addrEmptyVal, 200); delay(10);
    EEPROM.put(addrFullVal, 900); delay(10);
  }
  void LevelSensor::loadCalParameters() {
    EEPROM.get(addrEmptyVal, emptyMarkVal);
    EEPROM.get(addrFullVal, fullMarkVal);
  }
  // Save current sensor value as 0% (empty)
  void LevelSensor::calibrateEmpty() {
    emptyMarkVal = sensorRead();
    EEPROM.put(addrEmptyVal, emptyMarkVal); delay(10);
  }
  // Save current sensor value as 100% (full)
  void LevelSensor::calibrateFull() {
    fullMarkVal = sensorRead();
    EEPROM.put(addrFullVal, fullMarkVal); delay(10);
  }
  // Calculate tank level in percent
  float LevelSensor::getTankLevelPercent() {
    int currSensorVal=0; float levelPercentage=0.0;
    currSensorVal = sensorRead();
    //levelPercentage = floatMap(currSensorVal, emptyMarkVal, fullMarkVal, 0, 100);
    levelPercentage = (currSensorVal - emptyMarkVal) * 100.0 / (fullMarkVal - emptyMarkVal); // map function simplified for this case
    return levelPercentage;
  }
  int LevelSensor::sensorRead(){
    int sensorVal; unsigned int sumSensorVal; byte i;
    sensorVal = analogRead(sensorPin); // simply discard
    sumSensorVal = 0;
    for(i=0; i<6; i++){
      delay(200);
      sensorVal = analogRead(sensorPin);
      sumSensorVal += sensorVal;
    }
    sumSensorVal /= 6;
    sensorVal = sumSensorVal;
    return sensorVal;
  }
  // float LevelSensor::floatMap(float x, float in_min, float in_max, float out_min, float out_max) {
  //   float retVal=0;
  //   retVal = (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
  //   return retVal;
  // }

 // ShiftRegisterController class implementations
  ShiftRegisterController::ShiftRegisterController(int OE_Pin, int latchPin, int dataPin, int clkPin) {
    _OE_Pin = OE_Pin; _latchPin = latchPin; _dataPin = dataPin; _clkPin = clkPin;
    _doStartUpActions = true; _outputEnabled = false;
  }
  void ShiftRegisterController::updateOutputs(const byte DO_StatusArr[]) {
    if(_doStartUpActions){
      _doStartUpActions = false;
      pinMode(_OE_Pin, OUTPUT); enableOutput();
      pinMode(_latchPin, OUTPUT);
      pinMode(_dataPin, OUTPUT);
      pinMode(_clkPin, OUTPUT);
    }
    _dataByteDO = 0;
    for (byte i = 0; i < 8; i++) {
      if (DO_StatusArr[i] == 1) {
        _dataByteDO |= (1 << i);
      }
    }
    noInterrupts();
    digitalWrite(_latchPin, LOW);
    shiftOut(_dataPin, _clkPin, MSBFIRST, _dataByteDO);
    digitalWrite(_latchPin, HIGH);
    interrupts();
  }
  void ShiftRegisterController::disableOutput(){
    if(_outputEnabled){
      digitalWrite(_OE_Pin, HIGH); // _OE_Pin is active low
      _outputEnabled = false;
    }
  }
  void ShiftRegisterController::enableOutput(){
    if(!_outputEnabled){
      digitalWrite(_OE_Pin, LOW); // _OE_Pin is active low
      _outputEnabled = true;
    }
  }
