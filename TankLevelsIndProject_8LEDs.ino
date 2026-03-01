//#include <avr/wdt.h>
#include <SPI.h>
#include <EEPROM.h>
#include "CustomClasses.h"
#include <Ucglib.h>
#include <UtilityFunctions.h>
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
constexpr unsigned int MAIN_LOOP_CYCLE_PERIOD = 10000; // in milli seconds

enum navigations {dashBoardPage, slctTankPage, calSensorPage, setNumBeepsPage, setMidLvlBeepLenPage, showMessagePage, setDeviceModePage, noSgnlRcvdPage};
enum diplayPages {fullPage, updtValues}; enum deviceModes {masterMode, slaveMode};

// ------------------------------------------------- global variables ----------------------------------------------

//Ucglib_ST7735_18x128x160_SWSPI ucg(/*sclk=*/ 5, /*data=*/ 4, /*cd=*/ 3, /*cs=*/ -1, /*reset=*/ 2); //CS -> GND
Ucglib_ST7735_18x128x160_HWSPI ucg(/*cd=*/ 10, /*cs=*/ -1, /*reset=*/ 9); //CS -> GND, Pin 12(MISO) cannot be used for any purpose

extern const unsigned int TIMER1_PERIOD_MILLSEC;
Ticks tik_800ms(800); Ticks tik_1600ms(1600); Ticks tik_1000ms(1000); Ticks tik_main_loop_cycle(MAIN_LOOP_CYCLE_PERIOD);
Buzzer buzzer(A5);
NonBlockingTimer returnHomeScrOnTimeOut;
NonBlockingTimer charReceiveWaitingTimer;
NonBlockingTimer charReceiveFinishTimer;
NonBlockingTimer slaveModeMainLoopTimer;

Tank tank1(0); // EEPROM Address for storing status flags (2 byte alloc req)
Tank tank2(2); // EEPROM Address for storing status flags (2 byte alloc req)

LevelSensor levelSensor1(A6, 20); // EEPROM Address for storing calibration parameters (10 bytes alloc req)
LevelSensor levelSensor2(A7, 30); // EEPROM Address for storing calibration parameters (10 bytes alloc req)

ShiftRegisterController shiftOutDOsPort1(6, A0, 4, 5); // OE pin, latch pin, data pin, clock pin
ShiftRegisterController shiftOutDOsPort2(7, A1, 4, 5); // OE pin, latch pin, data pin, clock pin

CircularCounter LED_Num_Cir_Cnt(8); // for 8 No's LEDs

char arr16[17]; char arr8[9];
navigations currPage; deviceModes deviceMode;

float levelsPercentageFloat[2]; byte tankSel, numBeepsOnAlarm, beepLenMidLvl;
char levelsPercentageChar[7];

char buffer[bufferSize];

#define MACRO_BUTT_SCANS()              \
  if(currPage == dashBoardPage){        \
    readButtonLongPress(MID_BUT_PIN);   \
    readButtonLongPress(LHS_BUT_PIN);   \
    readButtonLongPress(RHS_BUT_PIN);   \
    readButtonLongPress(MODE_SET_BUT_PIN);  \
  }                                     \
  else{                                 \
    readButton(MID_BUT_PIN);            \
    readButton(LHS_BUT_PIN);            \
    readButton(RHS_BUT_PIN);            \
  }
// ----------------------------------------------------- Setup ------------------------------------------------------
void setup() {   // put your setup code here, to run once:
  delay(500);

  #if DEBUG_SERIAL_PRINT
    Serial.begin(115200);
  #else
    Serial.begin(1200);
  #endif
  sei();//allow interrupts
  analogReference(EXTERNAL); delay(500);
  
  pinMode(MID_BUT_PIN, INPUT_PULLUP);
  pinMode(LHS_BUT_PIN, INPUT_PULLUP);
  pinMode(RHS_BUT_PIN, INPUT_PULLUP);
  pinMode(MODE_SET_BUT_PIN, INPUT_PULLUP);

  arr16[16] = '\0'; arr8[8] = '\0';
  ucg.begin(UCG_FONT_MODE_SOLID);
  ucg.setRotate270();
  ucg.setColor(1, 0, 0, 0);        // Background color: Black (R,G,B)
  ucg.setColor(0, 255, 255, 255);  // Text color: White (R,G,B)

  if(erase_eeprom_if_req(EEPROM_ADDR_MEMORY_VALID_STRING)){
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

  setupTimer1(); enableTimer1_Int();
  tik_main_loop_cycle.force_Gen_Tick();
  
  //wdt_enable(WDTO_8S);
}
// ------------------------------------------------------ loop --------------------------------------------------------
void loop() {
  //put your main code here, to run repeatedly:
  if(deviceMode == masterMode && currPage == dashBoardPage){
    while(!tik_main_loop_cycle.tick_Utilize()){
      MACRO_BUTT_SCANS();
    }
  }

  if((deviceMode == slaveMode) && (currPage == dashBoardPage || currPage == noSgnlRcvdPage)){
    receiveCharLevels_And_Convert();
    slaveModeMainLoopTimer.startTimer(MAIN_LOOP_CYCLE_PERIOD-500);
  }

  MACRO_BUTT_SCANS();
  
  if(deviceMode == masterMode && currPage == dashBoardPage)
    levelsPercentageFloat[0] = levelSensor1.getTankLevelPercent();
  if(currPage == dashBoardPage)
    tank1.actionsOnLevel(levelsPercentageFloat[0]);
  
  MACRO_BUTT_SCANS();
  
  if(deviceMode == masterMode && currPage == dashBoardPage){
    levelsPercentageFloat[1] = levelSensor2.getTankLevelPercent();
  }
  if(currPage == dashBoardPage)
    tank2.actionsOnLevel(levelsPercentageFloat[1]);    
  
  MACRO_BUTT_SCANS();

  if(deviceMode == masterMode && currPage == dashBoardPage){
    convertLevelsToChar_And_Transmit();
  }
  if(currPage == dashBoardPage){
    shiftOutDOsPort1.updateOutputs(tank1.ledStatus);
    shiftOutDOsPort2.updateOutputs(tank2.ledStatus);
    displayHomePage(updtValues);
  }

  if(deviceMode == slaveMode && currPage == dashBoardPage){
    while(!slaveModeMainLoopTimer.checkTimeOut(false)){
      MACRO_BUTT_SCANS();
    } // while
  }

  if(returnHomeScrOnTimeOut.checkTimeOut(false)){
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
