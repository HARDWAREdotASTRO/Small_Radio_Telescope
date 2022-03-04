
//Set up count variable, last state variable and the count_pin
int count = 0;
int count_pin = 4;
int last_state = 0;

void setup() {
  // put your setup code here, to run once:

  //Start the serial
  Serial.begin(9600);

  pinMode(10, OUTPUT);  

  // 5V as long as circuit is closed, 0V when circuit open
  pinMode(4, INPUT);

  //output is shown on this pin
  pinMode(8, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:

  //read the sensor pin (pin that connects wire to switch)

  int sensorVal = digitalRead(4);

  //print value of sensor
  Serial.println(sensorVal);

  //check if the state is equal to the sensor value state.
  //If it is, don't increment count. If not, change value and increase by 1.
  if (last_state != sensorVal) {
    if (sensorVal == HIGH) {
      digitalWrite(10, HIGH); 
      digitalWrite(8, LOW);
      count += 1;
    }  else {
      digitalWrite(8, HIGH);
    }
  }

  digitalWrite(10, LOW); 

  //change state to recent state and print count
  last_state = sensorVal;
  Serial.println(count);
}
