# Arduino Test Files (In Progress) Created: 10/5/2021 at 1:09 PM 

These files were created to perform piecewise testing of different functions of the telescope. There were tests done to determine if the reed switch works as intended, limit switches
, the pololu chip and the motor itself. 

Changes that were made but not pushed in form of commits 

Below is a change to the reverse and forward direction files that was supposed to stop motor. Failed because the counter did not reach 2
until after the limit switch returned back to 5 V for pullup input. 
