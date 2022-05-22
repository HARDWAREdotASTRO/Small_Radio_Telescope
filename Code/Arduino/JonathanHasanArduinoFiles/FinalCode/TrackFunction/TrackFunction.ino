/*
 * Track function 
 * 
 * Author: Jonathan Hasan 
 * 
 * Date: 4/20/2022 
 * 
 * Purpose: Chooses a coordinate in the sky and follows it with telescope 
 */


void setup() {
  // put your setup code here, to run once:

// Need to communicate with both arduinos at the same time. Still need to resolve this. 

  //control variable 

  int motor_shutoff = 0; 

   //initialize pins and variables for switches 

  int count_pin_reed_azimuthal = 2; 

  int count_pin_reed_latitudinal = 3; 

  int reed_count_azimuthal = 0; 

  int reed_count_latitudinal = 0; 

  // Right Ascension and declination coordinates 
  
  float right_ascension = 0; 

  float declination = 0; 
  
  //initialize pins for the both motors 

  int A_1 = 13; 

  int A_2 = 12; 

  int A_PWM = 11; 

  int pin_clockwise = 6; 

  int val = 255; 
}

void loop() {
   // put your main code here, to run repeatedly:

  // need to find a way to communicate with astropy coordinates to move the telescope 

// if azimuthal count not the same as reed count move motor until reed count is acquired. 

  if(azimuthal_count > reed_count_azimuthal){
    //run code in clockwise direction 
  }

  else if(azimuthal_count < reed_count_azimuthal){
    //run code in counter clockwise direction
  }
// when acquired, print out Serial print 
  else{
    Serial.println("Right ascension coordinate acquired"); 
  }
// if latitudinal count is not the same as reed count move motor until reed count is acquired 

  if(latitudinal_count > reed_count_latitudinal){
    //run code in clockwise direction 
  }

  else if(latitudinal_count < reed_count_latitudinal){
    //run code in counter clockwise direction
  }
// if declination coordinate acquired print out 
  else{
    Serial.println("Declination coordinate acquired"); 
  }
}
