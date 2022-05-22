/*
 * Cumulative motor circuit arduino
 * 
 * Author: Jonathan Hasan 
 * Created: 11/4/2021
 * 
 * Purpose: 
 * 
 * Moves motors in desired direction depending on input 
 * 
 * 
 * 
 */

//Variables used defined here

//make sure GoHome activates before use!

int zeroed = 0; 

//tells arduino when to shutoff motor 
int motor_shutoff = 0; 

//contains direction of the motor 
int azimuth_direction = 0; 

//Initialize pins and variables for reed switch.
//One end of reed switch should be in pin 2, other in ground. Pin 8 is just for storing count variable 
int count_pin_reed = 2; 
int output_pin_reed = 8; 
int reed_count = 0; 
int last_state = 0; 

//pins initialized for the motor 

int A_1 = 13; 
int A_2 = 12; 
int A_PWM = 11; 

// pwm value 
int pwm_val = 255; 

//initialize pins and variables for limit switches

//reverse switch

int pin_reverse = 6; 
int output_pin_reverse = 4; 

//forward switch 

int pin_forward = 7; 
int output_pin_forward = 5; 

//Raspberry PI Data 

//Data inputs 
const byte numChars = 32;       //These establish a variable for the
char receivedChars[numChars];   //string being read from the Pi.

boolean newData = false;        //Reset for new data from the Pi.

//how far should the telescope go 
int count_till = 0; 

//Desired command from user
int order = 0; 
int request = 0; 



void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); 

  //setup the reed switch 

  pinMode(count_pin_reed, INPUT_PULLUP); 

  pinMode(output_pin_reed, OUTPUT); 

  //end setup of the reed switch 

  //setup for the motor
  
  pinMode(A_1, LOW); 
  digitalWrite(A_2, LOW); 
  analogWrite(A_PWM, LOW); 

  //end setup for the motor 

  //setup for limit switches
  pinMode(pin_forward, INPUT_PULLUP); 
  pinMode(pin_reverse, INPUT_PULLUP); 

  pinMode(output_pin_forward, OUTPUT);
  pinMode(output_pin_reverse,OUTPUT); 

  //end setup for limit switches

  //begin the homing process

  Serial.println("Motor starting...."); 

  Serial.println("Homing start"); 

  GoHome(motor_shutoff); 

  

}

void loop() {
  // put your main code here, to run repeatedly:

   SerialInputFromPi(); 
   translateString(request, order, count_till, pwm_val); 
   pickFunction(request); 
}





// Begin functions 

//Taking in Data From Raspberry Pi. Modified from Clayton Hanson's original code  

void SerialInputFromPi(){
  
  byte ndx = 0; 
  static char startMarker = '<'; 
  static char endMarker = '>'; 
  boolean newData = false; 
  while (Serial.available() > 0 && newData == false){
    if(Serial.available() == startMarker){
      char rc = Serial.available(); 
      fillBuffer(rc, ndx,  newData, endMarker);
    }
  }  
}

void fillBuffer(char rc, int ndx, boolean newData, char &endMarker ){
  Stop(); 

 //ternary operator 
 int c = (rc == endMarker && ndx <= numChars) ? 0 : 1;

  switch (c){
    case 0:
    //updates the buffer with new character at chosen index increments ndx by one
      receivedChars[ndx] = rc;
      ndx++;
      if(ndx >= numChars) {
        ndx = numChars-1; 
      }
      break;

    //sets final character to '\0' to seperate data and resets newData to get new Data 
    case 1: 
      receivedChars[ndx] = '\0'; 
      newData = true; 
      break; 
  }
}


//Take data from the raspberry pi and extract arguments for other functions. Source: Clayton Hanson 

int translateString(int &request, int &order, int &count_till, int &pwm_val){
  //Convert from ASCII to decimal if there is new data availible.
  if (newData == true){
     request = (receivedChars[0] & 0xf);             //Print pulse count.
     order = (receivedChars[2] & 0xf);               //Orders for motor.
     count_till = (receivedChars[4] & 0xf)*1000       //Count limit.
                      + (receivedChars[5] & 0xf)*100 
                      + (receivedChars[6] & 0xf)*10 
                      + (receivedChars[7] & 0xf);
     pwm_val = (receivedChars[9]& 0xf)*100               //PWM speed.
                  + (receivedChars[10]& 0xf)*10
                  +(receivedChars[11]& 0xf);
    
    newData = false;                                  //Data recieve
  }                                                   //complete.

  Serial.println("request: " + request); 
  Serial.println("order: " + order);
  Serial.println("Desired reed count: " + count_till); 
  Serial.println("Speed of PWM: " + pwm_val);  
}


//This is the go home function, will be responsible for setting the zero point for the telescope 
void GoHome(int motor_shutoff){

  int motor_shutoff_1 = motor_shutoff; 
  if(motor_shutoff_1 == 0){
  //As long as the control variable is 0, run the motor. 
    if(digitalRead(pin_forward) == LOW) {
      Serial.println("forward switch LOW"); 
      
      Stop(); 
          
      Forward(pwm_val);
      
      //When pin returns to INPUT_PULLUP, shutoff the motor 
      if(digitalRead(pin_forward) == HIGH){
        Serial.println("5V pullup forward reacquired"); 
        motor_shutoff_1 = 1; 
        Stop();
        Serial.println("shutting down"); 
        Serial.end();   
      }
    }

    // //Otherwise, run motor in forward direction at 50% duty cycle 
  else{
    Forward(pwm_val); 
   }
}

else{
  Stop(); 
  }

  azimuth_direction = 0;
  reed_count = 0;
  Serial.println("Successfully zeroed....");
}

// This function responsible for taking requests from the DASH Gui. Modified from claytons code to use a switch statement instead. 
int pickFunction(int &request){
  switch (order){
    case 0:
      azimuth_direction = 0;
      Stop(); 
      break; 
  
    case 1: 
      azimuth_direction = 1; 
      Move(reed_count, zeroed); 
      break;

    case 2:
      azimuth_direction = 2; 
      Move(reed_count, zeroed); 
      break; 

    case 3: 
       GoHome(motor_shutoff);
      break; 
}

if (request == 9){
    int k = counter();
    Serial.print(k);
    Serial.println(" ");
    request = 0;
  }
}


//Motor Movement functions

//This function controls movement of telescope. Modified from Clayton Hanson's original code. 
void Move(int &count, int &zeroed){

  
  int k = counter(); 
    if (k == count_till && zeroed == 1){
      azimuth_direction == 0;
    }
    
  switch(azimuth_direction){
    case 0:
      Stop();

    case 1:
      if (digitalRead(pin_forward) != LOW){
        Forward(pwm_val); 
      }
      else{
        Stop(); 
        delay(5); 
        count = 2001;
        zeroed = 1;  
      }

    //reverse direction ends up creating slack. Need to find a way to overshoot target and then reverse course in forward direction. 
    case 2:
      if(digitalRead(pin_reverse) != LOW){
        Reverse(pwm_val); 
      }

      else{
        Stop(); 
        delay(5); 
        count = 0;
        zeroed = 1;  
      }
    
  }
}

void Stop(){
  digitalWrite(A_1, LOW); 
  digitalWrite(A_2, LOW); 
  digitalWrite(A_PWM, LOW);
}

void Forward(int val){
  digitalWrite(A_1, LOW);
  digitalWrite(A_2, HIGH);
  analogWrite(A_PWM, round(val/2));
}

void Reverse(int val){
  digitalWrite(A_1, HIGH);
  digitalWrite(A_2, LOW);
  digitalWrite(A_PWM, round(val/2));
}

// function for counting 
int counter(){
  int reedSensorVal = digitalRead(count_pin_reed);

  if(last_state != reedSensorVal){
    if (reedSensorVal == HIGH){
      if (azimuth_direction == 1){
        reed_count ++; 
      }
      if(azimuth_direction == 2){
        reed_count--; 
      }
    }
  }
  last_state = reedSensorVal;
  return reed_count; 
}
  
