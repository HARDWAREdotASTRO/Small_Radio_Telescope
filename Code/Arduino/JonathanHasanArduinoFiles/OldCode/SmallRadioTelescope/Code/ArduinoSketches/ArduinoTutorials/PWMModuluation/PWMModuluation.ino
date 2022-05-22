int temp = 0;

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
  temp = analogRead(A0);
  temp = temp*(255.0/ 1023.0);
  analogWrite(11, temp); 
}
