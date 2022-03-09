void processInput(){
  switch(inputCommand[0]){    
    case 'D':
       PrintData();
    break;
    default:
      GetScaleCommand();
    break;    
  }
  
  memset(inputCommand, 0, sizeof(inputCommand));
  inputCounter = 0;  
}

void GetScaleCommand(){
  bool found = false;  
  for(int i = 0; i < NoOfSendModes; i++){    
    if(strlen(SendModes[i]) == inputCounter-1)
      for(int j = 0; j < inputCounter-1; j++)
        if(SendModes[i][j] == inputCommand[j]){          
          SendMode = i;
          found = true;          
        }
        else{
          found = false;
          break;
        }
    if(found)
      break;
  }

  if(found){
    CreateWeightString();
  }
  else{
    CreateUnknownString();    
  }
}

void PrintData(){
  simpleui.clear();
  for(int i = 0; i < inputCounter; i++){
    if(inputCommand[i+1] == 0x0D || i > 31)//CarriageReturn
      return;
    char c[] = {inputCommand[i+1], '\0'};    
    simpleui.overwrite(i/16, i - ((i/16) * 16), c);
//    Serial.print((char)inputCommand[i+1]);    
//    Serial.print(i/16);
//    Serial.print(":");
//    Serial.println(i - ((i/16) * 16));
//    delay(50);
  }    
}

void CreateWeightString(){  
  uint16_t i = random(MinIntegerPlaces,MaxIntegerPlaces);
  uint16_t d = random(MinDecimalPlaces,MaxDecimalPlaces);
  uint16_t i_net;
  uint16_t d_net;
  const char unit[] = "kg";
  char del[] = ".";
  if(Delimiter)
    del[0] = ',';
    
  //Net weight: (smaller then gross)
  if(i>MinIntegerPlaces)
    i_net = random(MinIntegerPlaces,i);
  else
    i_net = i;   
  
  if(d>MinDecimalPlaces)
    d_net = random(MinDecimalPlaces,d);
  else
    d_net = d;
  
  char weight[SendBufferLength];

  //Display Output
  sprintf(weight, "%5d%s%02d%s", i, del, d, unit);
  simpleui.overwrite(1,0,">               ");
  simpleui.overwrite(1,1,(char*)SendModes[SendMode]);
  if(inputCounter == 0)
    inputCounter = 4;
  simpleui.overwrite(1,inputCounter,weight);
  
  SendStart();

  switch(SendMode){
    case 0: //S
      //Echo
      Serial.print("S S ");
    break;
    case 7: //RN
    case 8: //RM
      //Error Code
      Serial.print("00");
    break;
    case 10: //GB
    case 11: //P
    case 12: //<FP>
      //Nothing
    break;
    default: //SXXX
      Serial.print(SendModes[SendMode]);
      for(int i = 0; i < 4 - inputCounter; i++)
        Serial.print(" ");    
  }  
         
  switch(SendMode){
    case 0: //S      
      sprintf(weight, "%5d%s%02d %s", i, del, d, unit);
    break;
    case 1: //S Dataset
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:    
      //sprintf interprets from right to left, so decrement weight no:
      CurrentWeightNo += 7;
      sprintf(weight, " A%.3d% 9d%s%.1d %s   A%.3d% 9d%s%.1d %s   A%.3d% 9d%s%.1d %s   A%.3d%8sA%.3d%42sA%.3d%17sA%.3d",
      CurrentWeightNo--, i, del, d, unit, CurrentWeightNo--, i_net, del, d_net, unit, CurrentWeightNo--, i-i_net, del, d-d_net, unit, CurrentWeightNo--, "", CurrentWeightNo--, "", CurrentWeightNo--, "", CurrentWeightNo--);
      CurrentWeightNo += 7;
    break;    
    case 7: //RN (standstill)
      delay(150);
    case 8: //RM (On the move/Immediately)    
      sprintf(weight, "0013.16.2113:37   11% 5d%s%.2d% 5d%s%.2d% 5d%s%.2d%s T 001   45678", i, del, d, i-i_net, del, d-d_net, i_net, del, d_net, unit);
    break;
    case 9: //<FP0>
      sprintf(weight, "% 10d%s%.2d %s% 10d%s%.2d %s% 10d%s%.2d %s", i, del, d, unit, i_net, del, d_net, unit, i-i_net, del, d-d_net, unit);
    break;
    case 10: //<GB1>
      sprintf(weight, "%5d%s%02d<GB>", i, del, d);
    break;
    case 11: //P
      //  Date:    18.05.03
      //Time:    16:28:42
      //Gross      0.40kg
      sprintf(weight, "  Date:    13.06.21\r\n  Time:    16:28:42\r\n  Gross      %1d%s%02d%s\r\n", i, del, d, unit);
    break;
    case 12: //<FP>
      sprintf(weight, "%c;     54993;13.06.2021;13:37:11;1;1;       0;%s;%6d%s%02d;%6d%s%.2d;%6d%s%02d;%c\r\n<FP>", 0x02, unit, i, del, d, i_net, del, d_net, i, del, d, 0x03);
    break;
  }

  if(PartialSendingMaxDelay)
    SendWeightPartially(weight);
  else
    Serial.print(weight);
  
  SendEnd();  
}

void CreateUnknownString(){
  simpleui.overwrite(1,0,"?               ");
  simpleui.overwrite(1,1,inputCommand);

  SendStart();
  Serial.print("? Unknown");
  SendEnd();
}

void SendStart(){
  switch(StartEndChar){    
    case 1:
    Serial.write(0x02);//STX
    break;
    case 2:
    Serial.print('<');
    break;
  }
}
void SendEnd(){
  switch(StartEndChar){    
    case 1:
    Serial.write(0x03);//ETX
    break;
    case 2:
    Serial.print('>');
    break;
  }

  //0 None, 1 CR, 2 LF, 3 CR+LF, 4 LF+CR
  switch(LineEnd){    
    case 1:
      Serial.print('\r');//CR
    break;
    case 2:
      Serial.print('\n');//LF
    break;
    case 3:    
      Serial.print('\r');//CR
      Serial.print('\n');//LF
    break;
    case 4:
      Serial.print('\n');//LF
      Serial.print('\r');//CR
    break;
  }
  Serial.flush();
}
void SendWeightPartially(char* weight){  
 for(uint8_t i = 0; i < SendBufferLength; i++){
  if(weight[i] == '\0')
    break;
  if(i % 2 == 0 && i > 10)
    delay(random(0,PartialSendingMaxDelay));
  Serial.print(weight[i]);
 }
}
