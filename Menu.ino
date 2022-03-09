void mainMenu(){
  char buffer[20];
  uint16_t i;
  char* menu[] = {"Listen","Send", "Setup", 0}; //last entry has to be 0
  switch(simpleui.showMenu("Scale Simulator",menu)){
    case 0:      
      ListenMode();
      break;
    case 1:      
    {         
       while(selectorMenu(SendMode, SendModes, NoOfSendModes, "Send:")){
        CreateWeightString();
        delay(500);
       }     
    }
      break;
    case 2:
      setupMenu();
      break;    
  }
}

void setupMenu(){
  char buffer[20];
  uint16_t i;
  char* menu[] = {"Values","Line End", "Start/End", "Delimiter", "Com-Port Baud", "Com-Port Setup", "Send Bytedelay", 0}; //last entry has to be 0
  uint8_t selection = simpleui.showMenu("Setup",menu);
  Serial.println("Selection: " + String(selection));
  switch(selection){
    case 0: //Values
      valuesMenu();
      break;
    case 1: //"Line End"
    {
      const char* submenu[] = {"None", "CarriageReturn", "Linefeed", "CR+LF", "LF+CR"};      
      if(selectorMenu(LineEnd, submenu, 5, menu[1])){
          simpleui.toString(LineEnd, buffer, 20);
          UpdateData(buffer);
        }
    }
      break;
    case 2://Start/End    
    {
      const char* submenu[] = {"None", "STX / ETX", "< / >"};      
      if(selectorMenu(StartEndChar, submenu, 3, menu[2])){
          simpleui.toString(StartEndChar, buffer, 20);
          UpdateData(buffer);
        }
    }    
      break;    
      case 3: //Delimiter
      {
        const char* submenu[] = {".", ","};      
        if(selectorMenu(Delimiter, submenu, 2, menu[3])){
          simpleui.toString(Delimiter, buffer, 20);
          UpdateData(buffer);
        }
      }
      break;
      case 4: //Com-Port Baud
      {
        ComPortBaudRate = simpleui.getUInt(ComPortBaudRate);
        simpleui.toString(ComPortBaudRate, buffer, 20);        
        UpdateData(buffer);
      }
      break;
      case 5: //Com-Port Setup
      {
        const char* submenu[] = {"5N1","6N1","7N1","8N1(Default)","5N2","6N2","7N2","8N2","5E1",
                        "6E1","7E1","8E1","5E2","6E2","7E2","8E2","5O1","6O1","7O1",
                        "8O1","5O2","6O2","7O2","8O2"};
        uint8_t i = SERIAL_8N1;
        if(ComPortSetup > 0)
          i = ComPortSetup / 2;         
        if(selectorMenu(i, submenu, 24, "Data Parity Stop"))
          if(ComPortSetup != i * 2)
          {          
            ComPortSetup = i * 2;
            simpleui.toString(ComPortSetup, buffer, 20);
            UpdateData(buffer);
            Serial.end();
            Serial.begin(ComPortBaudRate, ComPortSetup);
          }
      }
      break;
      case 6: //Send Byte delay
      {        
        PartialSendingMaxDelay = simpleui.getUInt(PartialSendingMaxDelay);
        simpleui.toString(PartialSendingMaxDelay, buffer, 20);        
        UpdateData(buffer);
      }
      break;      
  }
}

void valuesMenu(){
    char buffer[20];
  uint16_t i;
  char* menu[] = {"Min. places","Max. places", "Min. dec. places", "Max. dec. places", 0}; //last entry has to be 0
  switch(simpleui.showMenu("Values",menu)){
    case 0:      
      MinIntegerPlaces = simpleui.getUInt(MinIntegerPlaces);
      simpleui.toString(MinIntegerPlaces, buffer, 20);
      UpdateData(buffer);
      break;
    case 1:     
      MaxIntegerPlaces = simpleui.getUInt(MaxIntegerPlaces);
      simpleui.toString(MaxIntegerPlaces, buffer, 20);
      UpdateData(buffer);
      break;
    case 2:    
      MinDecimalPlaces = simpleui.getUInt(MinDecimalPlaces);
      simpleui.toString(MinDecimalPlaces, buffer, 20);
      UpdateData(buffer);
      break;     
    case 3:    
      MaxDecimalPlaces = simpleui.getUInt(MaxDecimalPlaces);
      simpleui.toString(MaxDecimalPlaces, buffer, 20);
      UpdateData(buffer);
      break;      
  }  
}

uint8_t selectedEntry = 0;
bool selectorMenu(uint8_t& value, const char* menu[], uint8_t entries, char* title){  
  selectedEntry = value;  
  while(true)
  {    
    if(title != ""){
      simpleui.write(title, (char*)menu[selectedEntry]);
      simpleui.overwrite(1,0, ">");
      simpleui.overwrite(1,1, (char*)menu[selectedEntry]);
    }
    else
      simpleui.write((char*)menu[selectedEntry]);
      
    switch(simpleui.waitButton()){
      case BUTTON_SELECT:
      {        
//        simpleui.write(menu[selectedEntry], "Selected");
//        delay(500);
        value = selectedEntry;
        return true;
      }
      break;  
      case BUTTON_UP:
        if(selectedEntry == 0)
          selectedEntry = entries-1;
        else
          selectedEntry--;        
      break;
      case BUTTON_DOWN:
        if(selectedEntry == entries -1)
          selectedEntry = 0;
        else
          selectedEntry++;        
      break;
      default:
      return false;
    }      
  }
}

bool selectorMenu(uint8_t& value, const char* menu[], uint8_t entries){  
  return selectorMenu(value, menu, entries, "");
}

void UpdateData(char buffer[20]){
  simpleui.write(buffer);
  StoreData();    
  simpleui.overwrite(1,0,"Stored");
  delay(1000);  
}

void ListenMode(){
  simpleui.write("Listen-Mode:");  
  while(true)
  {
    if(getButton() != BUTTON_NONE)
      return;    
    getInput();    
  }
}
