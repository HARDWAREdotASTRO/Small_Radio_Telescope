/*
 * Manual reed switch test code 
 * 
 * Purpose: Test the reed switch by manually spinning it in the presence of the reed switch. 
 * 
 * 
 * Instructions: 
 * Place magnet in plastic cutout and place reed switch underneath attached to pins 4 and ground. 
 * Test the reed switch by manually spinning it with your finger. Count should increment 
 */


//Set up variables 
int count = 0;          // reed switch count 
int count_pin = 4;      // pin arduino reads from 
//int output = 8;         //debug optional 
int last_state = 0;     // Holds last state to help increment counter 

void setup() {
  // put your setup code here, to run once:

  //Start the serial
  Serial.begin(9600);

  //pinMode(10, OUTPUT);  

  // 5V as long as circuit is closed, 0V when circuit open
  pinMode(count_pin, INPUT);

  //output is shown on this pin
  pinMode(output, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:

  //read the sensor pin (pin that connects wire to switch)

  int sensorVal = digitalRead(count_pin);

  //print value of sensor
  Serial.println(sensorVal);

  //check if the state is equal to the sensor value state.
  //If it is, don't increment count. If not, change value and increase by 1.
  if (last_state != sensorVal) {
    if (sensorVal == HIGH) {
      //digitalWrite(10, HIGH); 
      digitalWrite(output, LOW);
      count += 1;
    }  else {
      digitalWrite(output, HIGH);
      count += 0; 
    }
  }

  //digitalWrite(10, LOW); 

  //change state to recent state and print count
  last_state = sensorVal;
  Serial.println(count);
}
