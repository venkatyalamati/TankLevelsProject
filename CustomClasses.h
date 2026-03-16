#pragma once
#include <Arduino.h>
#include <EEPROM.h>

enum AlarmStatus : uint8_t {alarmNotReq, alarmStarted, alarmFinished};

class Tank{
private:
  int16_t _eepromStartAddr;
  bool _startAlarmOrder, _stopAlarmOrder, _beepForMidLvlChanges;
  uint8_t _levelStatus, _alarmStatus, _prevLevelStatus, _prevAlarmStatus;
  float _levelPercentage;
public:
  uint8_t ledStatus[8];
  
  Tank(int eepromStartAddr);
  void doStartUpActions();
  void actionsOnLevel(float levelPercentage);
  uint8_t update_ledStatus_getLevelStatus();
  bool getStartAlarmOrder();
  bool getStopAlarmOrder();
  bool isAlarmInProgress();
  bool beepForMidLvlChanges();
  void setAlarmStatusFinished();
};