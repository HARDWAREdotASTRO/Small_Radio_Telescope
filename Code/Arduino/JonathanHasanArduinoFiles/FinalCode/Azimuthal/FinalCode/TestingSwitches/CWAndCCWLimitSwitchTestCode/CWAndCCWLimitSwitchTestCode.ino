/*
 * Forward and Reverse Limit Switch testing 
 * 
 * Purpose: Try and detect activation events from the Left and Right Limit switches 
 * 
 * Instructions: Attach Left limit switch in the Normally open position and attach to pin 6 and ground. 
 * Attach Right Limit switch in the normally open position and attach to pin 7 and ground. 
 * Run the script and then toggle the switches to ensure the acquisition of appropriate counts 
 */



//Setup two count variables for forward and reverse
int rightLimitSwitchCount = 0;
int leftLimitSwitchCount = 0; 

int rightLimitSwitchPin = 7; 
int leftLimitSwitchPin = 6; 

int LastState1 = 0; 
int LastState2 = 0; 

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600); 

  pinMode(rightLimitSwitchPin, INPUT_PULLUP); 
  pinMode(leftLimitSwitchPin, INPUT_PULLUP); 

  pinMode(8, OUTPUT); 
  pinMode(9, OUTPUT); 

}

void loop() {
  // put your main code here, to run repeatedly:

  // Set variables to hold current state of limit switches 
  
  int SensorVal1 = digitalRead(rightLimitSwitchPin); 

  int SensorVal2 = digitalRead(leftLimitSwitchPin); 

  // If the Last state 1 variable is different and if the SensorVal1 is high then it is time to increment the right limit switch count 
  if(LastState1 != SensorVal1){
    if(SensorVal1 == HIGH) {
      //digitalWrite(8, LOW); 
      rightLimitSwitchCount += 1; 
    }
    //otherwise do nothing 
     else {
      //digitalWrite(8, HIGH); 
      rightLimitSwitchCount += 0; 
     }
  }
  // If the Last state 2 variable is different and if the SensorVal2 is high then it is time to increment the left limit switch count
  if(LastState2 != SensorVal2){
    if(SensorVal2 == HIGH) {
      //digitalWrite(9, LOW); 
      leftLimitSwitchCount += 1; 
    }
    //otherwise do nothing 
     else {
      //digitalWrite(9, HIGH);
      leftLimitSwitchCount += 0;  
     }
  }

  //set the Last State equal to sensor val to prepare for next change. 
  LastState1 = SensorVal1; 
  LastState2 = SensorVal2; 

  //print out current counts 
  Serial.println("Value for right Limit Switch Count is: " + String(rightLimitSwitchCount) + " " + "Value for left Limit Switch Count is: " + String(leftLimitSwitchCount)); 
  
  
}
