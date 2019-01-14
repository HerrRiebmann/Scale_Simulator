void InitializeData(){
  while (!eeprom_is_ready());
  
  SendMode = EEPROM.read(0);
  MinIntegerPlaces = (uint16_t)eepromReadInt(1);
  MaxIntegerPlaces = (uint16_t)eepromReadInt(3);
  MinDecimalPlaces = (uint16_t)eepromReadInt(5);
  MaxDecimalPlaces = (uint16_t)eepromReadInt(7);
  StartEndChar = EEPROM.read(9);
  LineEnd = EEPROM.read(10);
  Delimiter = EEPROM.read(11);  
  ComPortBaudRate = eepromReadInt(12);  
  ComPortSetup = EEPROM.read(14); 
}

void StoreData(){
  while (!eeprom_is_ready());
    
  EEPROM.update(0, SendMode);
  eepromWriteInt(1, MinIntegerPlaces); 
  eepromWriteInt(3, MaxIntegerPlaces);
  eepromWriteInt(5, MinDecimalPlaces);
  eepromWriteInt(7, MaxDecimalPlaces);
  EEPROM.update(9, StartEndChar);
  EEPROM.update(10, LineEnd);
  EEPROM.update(11, Delimiter);
  eepromWriteInt(12, ComPortBaudRate);  
  EEPROM.update(14, ComPortSetup);
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
