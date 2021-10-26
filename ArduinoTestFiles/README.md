# Arduino Test Files (In Progress) Created: 10/5/2021 at 1:09 PM 

These files were created to perform piecewise testing of different functions of the telescope. There were tests done to determine if the reed switch works as intended, limit switches
, the pololu chip and the motor itself. 

Changes that were made but not pushed in form of commits 

Below is a change to the reverse and forward direction files that was supposed to stop motor. Failed because the counter did not reach 2
until after the limit switch returned back to 5 V for pullup input. 

`
//if forward or reverse count greater than >= 2, stop the motor. 

//THIS NEEDS TO BE UPDATED! The limit switch must trigger a shutdown as soon as its driven to LOW 

//Code updated on 9/28/2021 at 5:08 PM 


  //if(forward_count >= 2  || reverse_count >= 2){
  //  digitalWrite(A_1, LOW);
  //  digitalWrite(A_2, LOW);
  //  digitalWrite(A_PWM, LOW);

    //if count for either forward or reverse exceeds 2, end the serial stream. 
  //  if(forward_count >=2){
  //    Serial.println("forward_count is greater than or equal to 2"); 
    //}
    //else{
    //  Serial.println("reverse_count is greater than or equal to 2"); 
   // }
   // Serial.println("Shutting down....");
   // Serial.end(); 
  //}

  //if(digitalRead(pin_forward) == LOW) {
  //  digitalWrite(A_1, LOW);
  //  digitalWrite(A_2, LOW);
  //  digitalWrite(A_PWM, LOW);
  //  Serial.println("forward limit switch triggered"); 
  //  Serial.println("Shutting down..."); 
  //  Serial.end(); 
 // }

 // if(digitalRead(pin_reverse) == LOW) {
 //   digitalWrite(A_1, LOW);
 //   digitalWrite(A_2, LOW);
 //   digitalWrite(A_PWM, LOW);
 //   Serial.println("reverse limit switch triggered"); 
//    Serial.println("Shutting down..."); 
 //   Serial.end(); 
 // }
`
