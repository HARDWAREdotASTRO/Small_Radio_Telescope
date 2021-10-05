/*
 * Motor Test Code with SRT switches, and reed switch in the forward direction 
 * 
 * The forward direction is Clockwise 
 * 
 * Author: Jonathan Hasan
 * Date: 6/24/2021
 * 
 * Updated : 6/28/2021 Time: 11:45 AM
 * 
 * Updated: 9/16/2021 Time: 3:13 PM
 * 
 * Updated on 9/28/2021 at 5:08 PM  - changed if statement for limit switch that immediately shuts off motor when forward or reverse limit switch is brought to LOW 
 * 
 * Updated: 9/30/2021 Time: 1:43 PM  - Got rid of the check for the reverse switch. WIll only stop if forward switch is triggered. 
 * 
 * 
 * Purpose: 
 * 
 * Create a script that will allow for the running of the motor with arduino, pololu chip
 * tsiny motor, the two limit switches on the SRT and the reed switch 
 *
 * 
 * Instructions:
 * 
 *Construct the circuit, install the limit switches, setup the motor and dc supply and then run the code. 
 */

//initialize pins and variables for reed switch 

//One end of reed switch should be in pin 2, other should be in ground. 
int count_pin_reed = 2;

//nothing connected to pin 8, just for storing count 
int output_pin_reed = 8; 
int reed_count = 0;
int last_state = 0;

//initialize pins for motor 

int A_1 = 13;       //These pins out put forward, reverse and the
int A_2 = 12;        //pulse width to the Pololu chip.
int A_PWM = 11;



// initialize pins and variables  for forward and reverse switch
int pin_forward = 7;
int pin_reverse = 6; 

int output_pin_forward = 5;
int output_pin_reverse = 4; 

int forward_count = 0; 
int reverse_count = 0; 


int last_state_forward = 0; 
int last_state_reverse = 0;

int val = 255; 


void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);         //Begin Serial connection.

// Setup for Reed Switch

  // 5V as long as circuit is open, 0V when circuit complete
  pinMode(count_pin_reed, INPUT_PULLUP);

  //output is shown on this pin
  pinMode(output_pin_reed, OUTPUT);

// End setup for Reed Switch

  // Setup for Motor 
  pinMode(A_1, OUTPUT);       //Initialize output pins.
  pinMode(A_2, OUTPUT);
  pinMode(A_PWM, OUTPUT);

  digitalWrite(A_1, LOW);     //Write output pins low so the
  digitalWrite(A_2, LOW);     //motors are off when the 
  analogWrite(A_PWM, LOW);   //Arduino is turned on.

  //End Setup for Motor 



// Setup for limit switches
  pinMode(pin_forward, INPUT_PULLUP); 
  pinMode(pin_reverse, INPUT_PULLUP); 

  pinMode(output_pin_forward, OUTPUT);
  pinMode(output_pin_reverse, OUTPUT); 

 // End setup for Limit Switches 

 Serial.println("Motor starting...."); 
  
}

void loop() {

// Motor Running. 


//if forward or reverse count greater than >= 2, stop the motor. 

 ///THIS CODE NEEDS TO BE UPDATED! The limit switch must stop as soon as it triggers to LOW. 

 //Code updated on 9/28/2021 at 5:08 PM 
 
  //if(forward_count >= 2  || reverse_count >= 2){
  //  digitalWrite(A_1, LOW);
  //  digitalWrite(A_2, LOW);
  //  digitalWrite(A_PWM, LOW);

    //if count for either forward or reverse exceeds 2, end the serial stream. 
  //  if(forward_count >=2){
  //    Serial.println("forward_count is greater than or equal to 2"); 
  //  }
  //  else{
  //    Serial.println("reverse_count is greater than or equal to 2"); 
  //  }
  //  Serial.println("Shutting down....");
  //  Serial.end(); 
 // }

 if(digitalRead(pin_forward) == LOW) {
    digitalWrite(A_1, LOW);
    digitalWrite(A_2, LOW);
    digitalWrite(A_PWM, LOW);
    
    Serial.println("forward limit switch triggered"); 
    Serial.println("Shutting down..."); 
    Serial.end(); 
  }

//forward direction shouldn't worry about reverse switch 
  //else if(digitalRead(pin_reverse) == LOW) {
  //  digitalWrite(A_1, LOW);
  //  digitalWrite(A_2, LOW);
  //  digitalWrite(A_PWM, LOW);
    
 //   Serial.println("reverse limit switch triggered"); 
 //   Serial.println("Shutting down..."); 
 //   Serial.end(); 
 // }
  
// End of Limit Switch check 

  //if reed switch count greater than 10, stop the motor.   # first 1000 second: 1029   Total reed switch counts : 2029 for azimuthal direction 
  else if(reed_count >= 10000){
    digitalWrite(A_1, LOW);
    digitalWrite(A_2, LOW);
    digitalWrite(A_PWM, LOW);

    Serial.println("Max reed count reached");
    Serial.println("Shutting down......");
    
    Serial.end(); 
  }
  
// else Start motor in forward direction with 50% duty cycle
  else{
    digitalWrite(A_1, HIGH); 
    digitalWrite(A_2, LOW); 
    digitalWrite(A_PWM, round(val/2)); 
   }

// Limit Switch counter

// Most likely don't need this. The last state variable can be used in order to keep track of behavior in event 
// power failure 

 ///THIS CODE NEEDS TO BE UPDATED! The limit switch must stop as soon as it triggers to LOW 
 
  //read the pin for switches
  int forwardSensorVal = digitalRead(pin_forward); 
  int reverseSensorVal = digitalRead(pin_reverse);


  //check if the state of pins 4, 5 is equal to the sensor value state. 
  //If it is, don't increment count. If not, change value and increase by 1. 
  if(last_state_forward != forwardSensorVal){
    if (forwardSensorVal == HIGH) {
      digitalWrite(output_pin_forward, LOW);
      forward_count += 1;
  }  else {
      digitalWrite(output_pin_forward, HIGH); 
    }
  }

  if(last_state_reverse != reverseSensorVal){
    if (reverseSensorVal == HIGH) {
      digitalWrite(output_pin_reverse, LOW);
      reverse_count += 1;
  }  else {
      digitalWrite(output_pin_reverse, HIGH); 
    }
  }


  //change state to recent state and print count
  last_state_forward = forwardSensorVal; 
  last_state_reverse = reverseSensorVal; 

   // end limit switch counter 
   

  // Reed Switch counter 

 //read the sensor pin (pin that connects wire to switch)

  int reedSensorVal = digitalRead(count_pin_reed);

  //print value of sensor
  //Serial.println(reedSensorVal);


  //check if the state is equal to the sensor value state.
  //If it is, don't increment count. If not, change value and increase by 1.
  if (last_state != reedSensorVal) {
    if (reedSensorVal == HIGH) {
      digitalWrite(output_pin_reed, LOW);
      reed_count += 1;
    }  else {
      digitalWrite(output_pin_reed, HIGH);
    }
  }

  //change state to recent state and print count
  last_state = reedSensorVal;
  Serial.println(reed_count);

// End Reed Switch Counter 



}
