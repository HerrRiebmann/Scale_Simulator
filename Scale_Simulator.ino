//Board:
//Arduino Duemilanove + LCD Shield//////////////////////////
#include <LiquidCrystal.h>
#include <SimpleUI16x2.h>
#include <EEPROM.h>

//CONFIGURATION/////////////////////////////////////////////
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

#define DEBUG 0

char inputCommand[33];
uint8_t inputCounter = 0;

#define SendBufferLength 250
#define NoOfSendModes 14
uint8_t SendMode = 0;
const char mode_01[] = {0x01};
const char* SendModes[] = {"S", "SI", "SIR", "SR", "SX", "SXI", "SXI", "RN", "RM", "FP0", "GB1", "P", "FP", mode_01};

//Settings:
uint16_t MinIntegerPlaces = 0;
uint16_t MaxIntegerPlaces = 999;

uint16_t MinDecimalPlaces = 0;
uint16_t MaxDecimalPlaces = 99;

uint8_t StartEndChar = 0; //0 None, 1 STX / ETX, 2 < / >

uint8_t LineEnd = 3; //0 None, 1 CR, 2 LF, 3 CR+LF, 4 LF+CR

uint8_t Delimiter = 0; //0: . | 1: ,

uint32_t ComPortBaudRate = 9600;

uint8_t ComPortSetup = SERIAL_8N1;

bool firstStart = true;

uint16_t CurrentWeightNo = 315;

uint8_t PartialSendingMaxDelay = 20; //0 = off

//EEPROM:
#define EEPROM_SendMode 0
#define EEPROM_MinIntegerPlaces 1
#define EEPROM_MaxIntegerPlaces 3
#define EEPROM_MinDecimalPlaces 5
#define EEPROM_MaxDecimalPlaces 7
#define EEPROM_StartEndChar 9
#define EEPROM_LineEnd 10
#define EEPROM_Delimiter 11
#define EEPROM_ComPortBaudRate 12
#define EEPROM_ComPortSetup 14
#define EEPROM_PartialSendingMaxDelay 16
#define EEPROM_CurrentWeightNo 17

//The Button function you have to define by yourself
uint8_t getButton() {
  int adc_key_in = analogRead(0);
  if (adc_key_in > 790) return BUTTON_NONE;
  if (adc_key_in < 50) {
    return BUTTON_RIGHT;
  }
  if (adc_key_in < 195) {
    return BUTTON_UP;
  }
  if (adc_key_in < 380) {
    return BUTTON_DOWN;
  }
  if (adc_key_in < 555) {
    return BUTTON_LEFT;
  }
  if (adc_key_in < 790) {
    return BUTTON_SELECT;
  }
  return BUTTON_NONE;
}
SimpleUI16x2 simpleui(&lcd, getButton);
//////////////////////////////////////////////////////////////

void setup() {  
  simpleui.write("Scale Simulator", "V1.2 by Thomas Rietschel");
  delay(1000);
  
#if !DEBUG  
  for (int i = 0; i <= 10; i++) {
    lcd.scrollDisplayLeft();
    delay(300);
  }
#endif

  InitializeData();
  Serial.begin(ComPortBaudRate, ComPortSetup);
  simpleui.clear();
  PrintStorage();
}

void loop() {
  if (firstStart) {
    firstStart = false;
    ListenMode();
  }
  mainMenu();
}

void getInput() {
  while (Serial.available())
  {
    char input = Serial.read();
    //Ignore when counter is empty:
    if (inputCounter == 0 && (input == 0x0D || input == 0x0A))
      continue;
    //Ignore Start/End chars
    if (input == '<' || input == '>' || input == 0x02 || input == 0x03)
      continue;
    inputCommand[inputCounter++] = input;
    //Carriage Return || Linefeed
    if (input == 0x0D || input == 0x0A)
      processInput();
  }
}
