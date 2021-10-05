/*
 * Motor Test Code with SRT switches
 * 
 * Author: Jonathan Hasan
 * Date: 6/24/2021
 * 
 * Updated : 6/28/2021 Time: 11:45 AM
 * 
 * Purpose: 
 * 
 * Create a script that will allow for the running of the motor with arduino, pololu chip
 * tsiny motor and the two limit switches on the SRT
 *
 * 
 * Instructions:
 * 
 *Construct the circuit, install the limit switches, setup the motor and dc supply and then run the code. 
 */


//initialize pins

int A_1 = 13;       //These pins out put forward, reverse and the
int A_2 = 12;        //pulse width to the Pololu chip.
int A_PWM = 11;



//pins for forward and reverse switch
int forward_pin = 7;
int reverse_pin = 6; 

//initialize variables 


//instantiate state variables
int last_state1 = 0; 
int last_state2 = 0; 


// Instantiate count variables 
int forward_count = 0; 
int reverse_count = 0; 

int val = 255; 


void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);         //Begin Serial connection.
  
  pinMode(A_1, OUTPUT);       //Initialize output pins.
  pinMode(A_2, OUTPUT);
  pinMode(A_PWM, OUTPUT);

  digitalWrite(A_1, LOW);     //Write output pins low so the
  digitalWrite(A_2, LOW);     //motors are off when the 
  analogWrite(A_PWM, LOW);   //Arduino is turned on.

//1. Uncomment below line when count_pin is instantiated
//  pinMode(count_pin, INPUT_PULLUP);  //Initialize sensor pins.

// Uncomment below lines when forward_pin and reverse_pin are instantiated
  pinMode(forward_pin, INPUT_PULLUP); 
  pinMode(reverse_pin, INPUT_PULLUP); 
 
  
}

void loop() {

// Condition for the running of the  motor. 

//if forward or reverse count greater than >= 1, stop the motor. 
  if(forward_count >= 2  || reverse_count >= 2){
    digitalWrite(A_1, LOW);
    digitalWrite(A_2, LOW);
    digitalWrite(A_PWM, LOW);

    //if count for either forward or reverse exceeds 2, end the serial stream. 
    if(forward_count >=2){
      Serial.println("forward_count is greater than or equal to 2"); 
    }
    else{
      Serial.println("reverse_count is greater than or equal to 2"); 
    }
    Serial.println("Shutting down....");
    Serial.end(); 
  }
// else Start motor in forward direction with 50% duty cycle
  else{digitalWrite(A_1, LOW); 
  digitalWrite(A_2, HIGH); 
  digitalWrite(A_PWM, round(val/2)); 
   }


  //read the pin for switches
  int sensorVal1 = digitalRead(forward_pin); 
  int sensorVal2 = digitalRead(reverse_pin);


  //check if the state of pins 8, 9 is equal to the sensor value state. 
  //If it is, don't increment count. If not, change value and increase by 1. 
  if(last_state1 != sensorVal1){
    if (sensorVal1 == HIGH) {
      digitalWrite(5, LOW);
      forward_count += 1;
  }  else {
      digitalWrite(5, HIGH); 
    }
  }

  if(last_state2 != sensorVal2){
    if (sensorVal2 == HIGH) {
      digitalWrite(4, LOW);
      reverse_count += 1;
  }  else {
      digitalWrite(4, HIGH); 
    }
  }


  //change state to recent state and print count
  last_state1 = sensorVal1; 
  last_state2 = sensorVal2; 
  



}
