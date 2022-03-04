/*
 * Adafruit Arduino - Lesson 3. RGB LED
 */

 int redPin = 11; 
 int greenPin = 10; 
 int bluePin = 9; 

void setup() {
  // put your setup code here, to run once:
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT); 
  pinMode(bluePin, OUTPUT); 
}

void loop() {
  // put your main code here, to run repeatedly:
  setColor(255,0,0); // red
  delay(1000); 
  setColor(0, 255,0); 
  delay(1000); 
  setColor(0,0, 255); 
  delay(1000);
  setColor(255,255,0);
  delay(1000);
  setColor(80,0,80);
  delay(1000);
  setColor(0, 255, 55);
  delay(1000);
}

void setColor(int red, int green, int blue)
{
  #ifdef COMMON_ANODE
  red = 255 - red;
  green = 255 - green;
  blue = 255 - blue;
  #endif
  analogWrite(redPin, red);
  analogWrite(greenPin, green); 
  analogWrite(bluePin, blue); 
}
