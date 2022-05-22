int A_1 = 13; 
int A_2 = 12; 
int A_PWM = 11; 
int val = 255; 
void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);

  pinMode(A_1, OUTPUT); 
  pinMode(A_2, OUTPUT); 
  pinMode(A_PWM, OUTPUT); 

  digitalWrite(A_1, HIGH); 
  digitalWrite(A_2, LOW); 
  analogWrite(A_PWM, val/2); 
  
}

void loop() {
  // put your main code here, to run repeatedly:

}
