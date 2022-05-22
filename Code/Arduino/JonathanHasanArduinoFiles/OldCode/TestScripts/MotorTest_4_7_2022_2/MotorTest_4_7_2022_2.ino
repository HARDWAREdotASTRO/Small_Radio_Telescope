
 int A_1 = 13; 

 int A_2 = 12;

 int A_PWM = 11;

 int val = 100; 
 
void setup() {
  // put your setup code here, to run once:


  // put your main code here, to run repeatedly:

   // Setup for Motor 
  pinMode(A_1, OUTPUT);       //Initialize output pins.
  pinMode(A_2, OUTPUT);
  pinMode(A_PWM, OUTPUT);

  digitalWrite(A_1, LOW);     //Write output pins low so the
  digitalWrite(A_2, LOW);     //motors are off when the 
  analogWrite(A_PWM, LOW);   //Arduino is turned on.

}

void loop() {
  
    digitalWrite(A_1, LOW); 
    digitalWrite(A_2, HIGH);
    digitalWrite(A_PWM, round(val/3));

}
