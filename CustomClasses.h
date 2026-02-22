#pragma once
#include <Arduino.h>
#include <EEPROM.h>
#include <UtilityClasses.h>
extern byte numBeepsOnAlarm, beepLenMidLvl;
class LevelSensor{
  public:
    int addrEmptyVal, addrFullVal, emptyMarkVal, fullMarkVal, sensorPin;
    LevelSensor(int sensorPin, int addrEEPROM);
    void storeDfltCalParameters();
    void loadCalParameters();
    void calibrateEmpty();
    void calibrateFull();
    float getTankLevelPercent();
    int sensorRead();
};

class ShiftRegisterController {
  public:
    ShiftRegisterController(int OE_Pin, int latchPin, int dataPin, int clkPin);
    void updateOutputs(const byte DO_StatusArr[]);
    void disableOutput();
    void enableOutput();

  private:
    int _OE_Pin, _latchPin, _dataPin, _clkPin;
    byte _dataByteDO; bool _doStartUpActions, _outputEnabled;
};

enum LevelAlarmStatus {alarmNotReq, highAlarmStarted, highAlarmFinished, lowAlarmStarted, lowAlarmFinished};

class Tank{
private:
  int _eepromStartAddr;
  bool _doStartUpActions;
  byte _levelStatus, _alarmStatus, _prevLevelStatus, _prevAlarmStatus;
  float _levelPercentage;
public:
  byte ledStatus[8];
  BlinkActivity ledBlinkAct;
  bool beepForMidLevels, alarmInProgress;

  Tank(int eepromStartAddr);
  void actionsOnLevel(float levelPercentage);
  uint8_t update_ledStatus_getLevelStatus();
};