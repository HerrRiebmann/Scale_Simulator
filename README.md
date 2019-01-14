# Scale_Simulator
Mock a scale via COM-Port (Arduino)

A simple simulator to test COM-Port applications.
Several things can be setup on the device, like:
* 12 different Modes
* Min./Max. values
* No. of decimal places
* Baudrate, Parity, ect.
* Delimeter (. / ,)
* Start-/Endchar (STX / ETX / < / >)

### Hardware
* Arduino Duemilanove
* LCD Keypad Shield

### Libraries
* LiquidCrystal
* [SimpleUI16x2](https://github.com/dserv01/SimpleUI16x2)
* EEPROM

### Sample Outputs
```
S S     4.08 kg
SI A011        8.1 kg   A012        0.0 kg   A013        8.1 kg   A310        A311                                          A312                 A019
SIR A011        6.8 kg   A012        3.7 kg   A013        3.1 kg   A310        A311                                          A312                 A019
SR A011        7.7 kg   A012        4.3 kg   A013        3.4 kg   A310        A311                                          A312                 A019
SX A011        6.1 kg   A012        3.0 kg   A013        3.1 kg   A310        A311                                          A312                 A019
SXI A011        4.6 kg   A012        3.0 kg   A013        1.6 kg   A310        A311                                          A312                 A019
RN:
000013.16.1813:37   11    8.07    7.04    1.03kg T 001   45678
RM:
000013.16.1813:37   11    5.01    5.01    0.00kg T 001   45678
 FP0         1.01 kg         0.00 kg         1.01 kg
GB1:
 1.02<GB>
P:
  Date:    18.05.03
  Time:    16:28:42
  Gross      2.02kg
```
