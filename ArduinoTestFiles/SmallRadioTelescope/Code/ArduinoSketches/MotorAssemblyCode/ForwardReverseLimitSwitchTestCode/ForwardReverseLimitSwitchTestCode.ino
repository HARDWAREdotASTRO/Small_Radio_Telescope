//Setup two count variables for forward and reverse
int ForCount = 0;
int RevCount = 0; 

int ForwardPin = 6; 
int ReversePin = 7; 

int LastState1 = 0; 
int LastState2 = 0; 

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600); 

  pinMode(ForwardPin, INPUT_PULLUP); 
  pinMode(ReversePin, INPUT_PULLUP); 

  pinMode(8, OUTPUT); 
  pinMode(9, OUTPUT); 

}

void loop() {
  // put your main code here, to run repeatedly:
  int SensorVal1 = digitalRead(ForwardPin); 

  int SensorVal2 = digitalRead(ReversePin); 

  if(LastState1 != SensorVal1){
    if(SensorVal2 == HIGH) {
      digitalWrite(8, LOW); 
      ForCount += 1; 
    }
     else {
      digitalWrite(8, HIGH); 
     }
  }

  if(LastState2 != SensorVal2){
    if(SensorVal2 == HIGH) {
      digitalWrite(9, LOW); 
      ForCount += 1; 
    }
     else {
      digitalWrite(9, HIGH); 
     }
  }

  LastState1 = SensorVal1; 
  LastState2 = SensorVal2; 

  if(ForCount > 0){
    Serial.println("Value for forward count is: " + String(ForCount)); 

  if(RevCount > 0){
    Serial.println("Value for reverse count is: " + String(RevCount)); 
    }
  }
}
