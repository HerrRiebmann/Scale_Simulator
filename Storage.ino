void InitializeData(){
  while (!eeprom_is_ready());
  
  SendMode = EEPROM.read(EEPROM_SendMode);
  MinIntegerPlaces = (uint16_t)eepromReadInt(EEPROM_MinIntegerPlaces);
  MaxIntegerPlaces = (uint16_t)eepromReadInt(EEPROM_MaxIntegerPlaces);
  MinDecimalPlaces = (uint16_t)eepromReadInt(EEPROM_MinDecimalPlaces);
  MaxDecimalPlaces = (uint16_t)eepromReadInt(EEPROM_MaxDecimalPlaces);
  StartEndChar = EEPROM.read(EEPROM_StartEndChar);
  LineEnd = EEPROM.read(EEPROM_LineEnd);
  Delimiter = EEPROM.read(EEPROM_Delimiter);  
  ComPortBaudRate = eepromReadInt(EEPROM_ComPortBaudRate);  
  ComPortSetup = EEPROM.read(EEPROM_ComPortSetup);
  PartialSendingMaxDelay = EEPROM.read(EEPROM_PartialSendingMaxDelay);
  CurrentWeightNo = eepromReadInt(EEPROM_CurrentWeightNo);
}

void StoreData(){
  while (!eeprom_is_ready());
    
  EEPROM.update(EEPROM_SendMode, SendMode);
  eepromWriteInt(EEPROM_MinIntegerPlaces, MinIntegerPlaces); 
  eepromWriteInt(EEPROM_MaxIntegerPlaces, MaxIntegerPlaces);
  eepromWriteInt(EEPROM_MinDecimalPlaces, MinDecimalPlaces);
  eepromWriteInt(EEPROM_MaxDecimalPlaces, MaxDecimalPlaces);
  EEPROM.update(EEPROM_StartEndChar, StartEndChar);
  EEPROM.update(EEPROM_LineEnd, LineEnd);
  EEPROM.update(EEPROM_Delimiter, Delimiter);
  eepromWriteInt(EEPROM_ComPortBaudRate, ComPortBaudRate);  
  EEPROM.update(EEPROM_ComPortSetup, ComPortSetup);
  EEPROM.update(EEPROM_PartialSendingMaxDelay, PartialSendingMaxDelay);
  eepromWriteInt(EEPROM_CurrentWeightNo, CurrentWeightNo);
#if DEBUG
  Serial.println("Stored!");
  PrintStorage();
#endif
}

void eepromWriteInt(int adr, int value) {
  byte low, high;
  low=value&0xFF;
  high=(value>>8)&0xFF;
  EEPROM.update(adr, low);
  EEPROM.update(++adr, high);
  return;
}

int eepromReadInt(int adr) {
byte low, high;
  low=EEPROM.read(adr);
  high=EEPROM.read(++adr);
  return low + ((high << 8)&0xFF00);
}

void PrintStorage(){
#if DEBUG  
  Serial.print("SendMode: ");
  Serial.println(SendMode);
  Serial.print("Int: ");
  Serial.print(MinIntegerPlaces);
  Serial.print(" - ");
  Serial.println(MaxIntegerPlaces);
  Serial.print("Dec: ");
  Serial.print(MinDecimalPlaces);
  Serial.print(" - ");
  Serial.println(MaxDecimalPlaces);
  Serial.print("StartEndChar: ");
  Serial.println(StartEndChar);
  Serial.print("LineEnd: ");
  Serial.println(LineEnd);
  Serial.print("Delimiter: ");
  Serial.println(Delimiter);  
  Serial.print("ComPortBaudRate: ");
  Serial.println(ComPortBaudRate);  
  Serial.print("ComPortSetup: ");
  Serial.println(ComPortSetup); 
  Serial.print("Byte sending delay: ");
  Serial.println(PartialSendingMaxDelay); 
  Serial.print("Current Weight No: ");
  Serial.println(CurrentWeightNo); 
#endif
}
