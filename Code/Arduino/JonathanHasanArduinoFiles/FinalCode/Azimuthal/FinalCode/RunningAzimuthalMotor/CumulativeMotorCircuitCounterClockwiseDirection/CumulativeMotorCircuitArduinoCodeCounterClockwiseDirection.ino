/*
 * Motor Test Code with limit switches and reed switch in the CounterClockwise direction 
 * 
 * 
 * Author: Jonathan Hasan
 * Created: 4/11/2022
 * 
 * Purpose: 
 * 
 * Run the motor counterclockwise using the pololu motor carrier chip 
 *
 * 
 * Instructions:
 * 
 *Construct the circuit according to wiring document, install the limit switches,install reed switch setup the motor and dc supply and then run the code. 
 */

 
//control variable for conditional statements 

int motor_shutoff = 0; 

//initialize pins and variables for reed switch. 
//One end of reed switch should be in pin 2, other should be in ground. Pin 8 is just for storing count variable
int count_pin_reed = 2;
int output_pin_reed = 8; 
int reed_count = 0;
int last_state = 0;

//initialize pins for motor 

int A_1 = 13;       //These pins out put forward, reverse and the
int A_2 = 12;        //pulse width to the Pololu chip.
int A_PWM = 11;

// initialize pins and variables for reverse switch . Originally 6. output_pin_reverse most likely used for debugging in claytons original code. 
int pin_reverse = 7; 
//int output_pin_reverse = 4; 

//set pwm value 
int val = 255; 


void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);         //Begin Serial connection.

// Setup for Reed Switch

  // 5V as long as circuit is open, 0V when circuit complete
  pinMode(count_pin_reed, INPUT_PULLUP);

  //output of reed switch is shown on this pin
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
  pinMode(pin_reverse, INPUT_PULLUP); 
//  pinMode(output_pin_reverse, OUTPUT); 

 // End setup for Limit Switches 

  Serial.println("Motor Reverse Test starting...."); 
}

void loop() {

// Motor Running. 

//As long as the control variable is 0, run the motor. 
if(motor_shutoff == 0){
  
  //if reed switch count less than -300000, stop the motor.
   
  if(reed_count <= -30000){
    digitalWrite(A_1, LOW);
    digitalWrite(A_2, LOW);
    digitalWrite(A_PWM, LOW);
    motor_shutoff = 1; 
    Serial.println("Minimum reed count reached");
    Serial.println("Shutting down......");
    
    Serial.end(); 
  }

  // End of Limit Switch Check 

  // if the limit switch is triggered, stop the motor and then reverse its direction
  else if(digitalRead(pin_reverse) == LOW){
    Serial.println("reverse switch LOW"); 
    digitalWrite(A_1, LOW);
    digitalWrite(A_2, LOW);
    digitalWrite(A_PWM, LOW);
    
    digitalWrite(A_1, HIGH); 
    digitalWrite(A_2, LOW);
    digitalWrite(A_PWM, round(val/2));

   //When pin returns to INPUT_PULLUP- shutoff motor 
    if(digitalRead(pin_reverse) == HIGH){
      Serial.println("5V pull up reverse reacquired");
      motor_shutoff = 1; 
      digitalWrite(A_1, LOW);
      digitalWrite(A_2, LOW);
      digitalWrite(A_PWM, LOW);
      Serial.println("Shutting down"); 
      Serial.end();
      
    }
  }

  //Otherwise, run motor in reverse direction at 50% duty cycle 
  else{
    //delay(100); 
    digitalWrite(A_1, LOW); 
    digitalWrite(A_2, HIGH); 
    digitalWrite(A_PWM, round(val/2));


  }
}

////If control variable is 1, shutoff motor for all future loops  
  else{
  digitalWrite(A_1, LOW); 
  digitalWrite(A_2, LOW); 
  digitalWrite(A_PWM, LOW); 
  }




// Reed Switch counter 

  //read the reed sensor pin (pin that connects wire to switch)

  int reedSensorVal = digitalRead(count_pin_reed);

  //print value of sensor
  Serial.println(reedSensorVal);


  //check if the state is equal to the sensor value state.
  //If it is, don't increment count. If not, change value and increase by 1.
  if (last_state != reedSensorVal) {
    if (reedSensorVal == HIGH) {
      //digitalWrite(output_pin_reed, LOW);
        reed_count -= 1;
        } 
    else {
        //digitalWrite(output_pin_reed, HIGH);
        reed_count -= 0; 
        }
   }

  //change state to recent state and print count
  last_state = reedSensorVal;
  Serial.println(reed_count);

// End Reed Switch Counter 
}
