#include <EEPROM.h>
#include "CustomClasses.h"
#include <UtilityClasses.h>
/*--------------------------------------------------- instructions --------------------------------------------------
1. HC-12 Set Pin must be connected to Vcc for regular transmission or receive
2. Both of the HC-12 deviecs have been configured as follows:
    10:44:49.274 -> OK+B1200
    10:44:49.274 -> OK+RC001
    10:44:49.356 -> OK+RP:+20dBm
    10:44:49.424 -> OK+FU4
3. When you Serial.print() a char array (i.e., a C-style string), the first character printed is the one at index 0

----------------------------------------------------- instructions -------------------------------------------------*/

// --------------------------------------------------- definitions --------------------------------------------------

#define DEBUG_SERIAL_PRINT false

// #ifdef U8X8_HAVE_HW_SPI
// #include <SPI.h>
// #endif

#define MID_BUT_PIN 2
#define LHS_BUT_PIN A2
#define RHS_BUT_PIN 3
#define MODE_SET_BUT_PIN 8 // shown as ALARM BUTTON on PCB but is used for setting MASTER/SLAVE mode

#define EEPROM_ADDR_DEVICE_MODE 100 // one byte
#define EEPROM_ADDR_NUM_BEEPS_ON_ALARM 101 // one byte
#define EEPROM_ADDR_BEEP_LEN_MID_LVL 102 // one byte

#define EEPROM_ADDR_MEMORY_VALID_STRING 1000

#define bufferSize 14
constexpr uint16_t MAIN_LOOP_CYCLE_PERIOD = 10000; // in milli seconds

enum navigations : uint8_t {dashBoardPage, slctTankPage, calSensorPage, setNumBeepsPage, setMidLvlBeepLenPage, showMessagePage, setDeviceModePage, noSgnlRcvdPage};
enum diplayPages : uint8_t {fullPage, updtValues}; enum deviceModes : uint8_t {masterMode, slaveMode};

// ------------------------------------------------- global variables ----------------------------------------------

TFTDisplay tftST7735(10, 9);

Ticks tik_1000ms(1000); Ticks tik_main_loop_cycle(MAIN_LOOP_CYCLE_PERIOD);
Buzzer buzzer(A5);

NonBlockingTimer returnHomeScreenTimer;
NonBlockingTimer charReceiveMainTimer;
NonBlockingTimer charReceiveSubTimer;
NonBlockingTimer slaveModeMainLoopTimer;

Tank tank1(0); // EEPROM Address for storing status flags (2 byte alloc req)
Tank tank2(2); // EEPROM Address for storing status flags (2 byte alloc req)

LevelSensor levelSensor1(A6, 20); // EEPROM Address for storing calibration parameters (10 bytes alloc req)
LevelSensor levelSensor2(A7, 30); // EEPROM Address for storing calibration parameters (10 bytes alloc req)

ShiftRegisterController shiftOutDOsPort1(6, A0, 4, 5); // OE pin, latch pin, data pin, clock pin
ShiftRegisterController shiftOutDOsPort2(7, A1, 4, 5); // OE pin, latch pin, data pin, clock pin

PreemptiveOnOff tank1AlarmOnOff(1600, 50);
PreemptiveOnOff tank2AlarmOnOff(1600, 50);

CircularCounter LED_Num_Cir_Cnt(8); // for 8 No's LEDs

navigations currPage; deviceModes deviceMode;

float levelsPercentageFloat[2]; uint8_t tankSel, numBeepsOnAlarm, beepLenMidLvl;
char levelsPercentageChar[7]; char buffer[bufferSize];

// Here the compiler allocating actual memory for the static variables. This is allowed even for private scope variables of the class.
// static keyword only in declaration inside class — never repeat it in definition outside class.
volatile uint16_t ButtonTimer2Based::timer2_isr_tick = 0; // All unsigned integer types wrap around correctly in C++
volatile uint16_t ButtonTimer2Based::debounceTicks = 255;
volatile uint8_t ButtonTimer2Based::pinButtPressed = 255;

ButtonTimer2Based mid_but_pin(MID_BUT_PIN);
ButtonTimer2Based lhs_but_pin(LHS_BUT_PIN);
ButtonTimer2Based rhs_but_pin(RHS_BUT_PIN);
ButtonTimer2Based mode_set_but_pin(MODE_SET_BUT_PIN);

// ----------------------------------------------------- Setup ------------------------------------------------------
void setup() {   // put your setup code here, to run once:
  delay(1000);
  
  #if DEBUG_SERIAL_PRINT
    Serial.begin(115200);
  #else
    Serial.begin(1200);
  #endif
  sei();//allow interrupts
  analogReference(EXTERNAL);
  
  mid_but_pin.setPinMode();
  lhs_but_pin.setPinMode();
  rhs_but_pin.setPinMode();
  mode_set_but_pin.setPinMode();

  tank1.doStartUpActions();
  tank2.doStartUpActions();

  shiftOutDOsPort1.doStartUpActions();
  shiftOutDOsPort2.doStartUpActions();

  tftST7735.begin();
  buzzer.begin();

  if(EEPROM_Functions::erase_eeprom_if_req(EEPROM_ADDR_MEMORY_VALID_STRING)){
    levelSensor1.storeDfltCalParameters();
    levelSensor2.storeDfltCalParameters();
    storeDfltBeepSettings();
    storeDfltDeviceModeSetting();
  }
  else{
    levelSensor1.loadCalParameters();
    levelSensor2.loadCalParameters();
    loadBeepSettings();
    loadDeviceModeSetting();
  }
  
  displayHomePage(fullPage); blinkLEDsInSeq();

  ArduinoTimersFunctions::setupTimer1(); ArduinoTimersFunctions::enableTimer1();
  ArduinoTimersFunctions::setupTimer2(); ArduinoTimersFunctions::enableTimer2();
  tik_main_loop_cycle.force_Gen_Tick();
}
// ------------------------------------------------------ loop --------------------------------------------------------
void loop() {
  //put your main code here, to run repeatedly:
  if(currPage == dashBoardPage){
    ButtonTimer2Based::setDebLongPress();
  }
  else{
    ButtonTimer2Based::setDebNormPress();
  }

  if(deviceMode == masterMode && currPage == dashBoardPage){
    while(!tik_main_loop_cycle.tick_Utilize()){
      actionOnButtonPress(ButtonTimer2Based::pinButtPressed); 
    }
  }

  if((deviceMode == slaveMode) && (currPage == dashBoardPage || currPage == noSgnlRcvdPage)){
    receiveCharLevels_And_Convert();
    slaveModeMainLoopTimer.start_finOneShot(MAIN_LOOP_CYCLE_PERIOD-500);
  }

  actionOnButtonPress(ButtonTimer2Based::pinButtPressed);
  
  if(deviceMode == masterMode && currPage == dashBoardPage)
    levelsPercentageFloat[0] = levelSensor1.getTankLevelPercent();
  if(currPage == dashBoardPage){
    tank1.actionsOnLevel(levelsPercentageFloat[0]);
    if(tank1.getStartAlarmOrder()) tank1AlarmOnOff.start(numBeepsOnAlarm);
    if(tank1.getStopAlarmOrder()) tank1AlarmOnOff.stop();
  }
  actionOnButtonPress(ButtonTimer2Based::pinButtPressed);
  
  if(deviceMode == masterMode && currPage == dashBoardPage){
    levelsPercentageFloat[1] = levelSensor2.getTankLevelPercent();
  }
  if(currPage == dashBoardPage){
    tank2.actionsOnLevel(levelsPercentageFloat[1]);
    if(tank2.getStartAlarmOrder()) tank2AlarmOnOff.start(numBeepsOnAlarm);
    if(tank2.getStopAlarmOrder()) tank2AlarmOnOff.stop();
  }
  actionOnButtonPress(ButtonTimer2Based::pinButtPressed);

  if(deviceMode == masterMode && currPage == dashBoardPage){
    convertLevelsToChar_And_Transmit();
  }
  if(currPage == dashBoardPage){
    shiftOutDOsPort1.updateOutputs(tank1.ledStatus);
    shiftOutDOsPort2.updateOutputs(tank2.ledStatus);
    displayHomePage(updtValues);
  }

  if(deviceMode == slaveMode && currPage == dashBoardPage){
    while(!slaveModeMainLoopTimer.event()){
      actionOnButtonPress(ButtonTimer2Based::pinButtPressed);
    } // while
  }

  if(returnHomeScreenTimer.event()){
    loadDeviceModeSetting();
    loadBeepSettings();
    displayHomePage(fullPage);
  }
  
} // ------------------------------------------------- main loop ends ----------------------------------------------

// --------------------------------------------------- misc functions ----------------------------------------------

#if DEBUG_SERIAL_PRINT
  template <typename T>
  void sp(char title[], T value){
    Serial.print(title); Serial.print(" "); Serial.println(value);
  }
#endif

void blinkLEDsInSeq(){
  uint8_t ledStatus[8];
  for(uint8_t i=0; i<8; i++)
    ledStatus[i] = 0;
  for(uint8_t i=0; i<8; i++){
    ledStatus[i] = 1;
    shiftOutDOsPort1.updateOutputs(ledStatus);
    shiftOutDOsPort2.updateOutputs(ledStatus);
    delay(1000);
  }
}

void blinkLEDs_usingTimerISR(){
  uint8_t ledStatus[8];
  LED_Num_Cir_Cnt.enableCnt();
  if(LED_Num_Cir_Cnt.checkJustIncr()){
    for(uint8_t i=0; i<8; i++){
      if(i==LED_Num_Cir_Cnt.count)
        ledStatus[i] = 1;
      else
        ledStatus[i] = 0;
    }
    shiftOutDOsPort1.updateOutputs(ledStatus);
    shiftOutDOsPort2.updateOutputs(ledStatus);
  }
}

void clearAll_LEDs(){
  uint8_t ledStatus[8];
  for(uint8_t i=0; i<8; i++){
    ledStatus[i] = 0;
  }
  LED_Num_Cir_Cnt.disableCnt();
  shiftOutDOsPort1.updateOutputs(ledStatus);
  shiftOutDOsPort2.updateOutputs(ledStatus);
}
