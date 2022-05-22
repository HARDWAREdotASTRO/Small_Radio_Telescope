/*
 * Motor Test Code
 * 
 * Author: Jonathan Hasan
 * Date: 6/24/2021
 * 
 * Updated: 4/22/2022
 * 
 * Purpose: 
 * 
 * Create a script that will allow for the running of the motor with arduino, pololu chip
 * and tsiny motor with a reed switch in the clockwise direction. 
 * 
 * If you want to go in the opposite direction, switch the values of digitalWrite A1 and A2 in else block 
 */


//initialize pins

int A_1 = 13;       //These pins out put forward, reverse and the
int A_2 = 12;        //pulse width to the Pololu chip.
int A_PWM = 11;

//reed switch pin variable
int count_pin = 2; 

//initialize variables for checking state of reed switch
int last_state = 0; 
int count = 0; 
int max_count = 50;  

//pwm value
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

  pinMode(count_pin, INPUT_PULLUP);  //Initialize reed sensor pin.

  Serial.println("Starting Reed Switch Test...."); 
}

void loop() {

// Condition for the running of the  motor. 
// If count >= 10; the motor stops and won't be permitted to run. 

//if switch count greater than max, stop the motor. 
  if(count >= max_count){

    Serial.println("Max count reached, shutting down..."); 
    digitalWrite(A_1, LOW);
    digitalWrite(A_2, LOW);
    digitalWrite(A_PWM, LOW);
    Serial.end(); 
  }
// else Start motor in clockwise direction with 50% duty cycle
   else{
    
  digitalWrite(A_1, LOW); 
  digitalWrite(A_2, HIGH); 
  digitalWrite(A_PWM, round(val/2)); 
   }


  //read the pin for switch
  int sensorVal = digitalRead(count_pin); 

  //print value for count pin
  Serial.println(sensorVal); 

  //check if the state of pin 8 is equal to the sensor value state. 
  //If it is, don't increment count. If not, change value and increase by 1. 
  if(last_state != sensorVal){
    if (sensorVal == HIGH) {
      //digitalWrite(8, LOW);
      count += 1;
  }  else {
      //digitalWrite(8, HIGH); 
      count += 0; 
    }
  }


  //change state to recent state and print count
  last_state = sensorVal; 
  Serial.println(count); 

  

}
