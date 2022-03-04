int count = 10; 
int RevLimit = 6;   //CCW Pins for limit switches to stop motors at
int ForLimit = 7;   //CW 0 and 180 degrees.
int forward_check;  //Stores the digital read to check against 
int reverse_check;  //if statements.
int revLimitReach = 0; //Reverse limit hasn't been reached.
int forLimitReach = 0; //Forward limit hasn't been reached.
int zeroed = 0;
int order = 0; 
int k; 

int Stow(int stow_count, int &direct, int &forLimitReach, int &revLimitReach){

    if (count > (stow_count + 1)){
      if (forLimitReach == 0){
        direct = 1; 
        Move(count, forLimitReach,revLimitReach, zeroed); 
      }
      else{
        direct = 2;
        Move(count, forLimitReach, revLimitReach, zeroed);  
      }
    }

    else if (count < (stow_count - 1)) {
      if (revLimitReach == 0){
        direct = 2;
        Move(count, forLimitReach, revLimitReach, zeroed);
      }
      else {
        direct = 1; 
        Move(count, forLimitReach,revLimitReach, zeroed); 
      }
    }
    else{
      direct = 0;
      forLimitReach = 0;
      revLimitReach = 0;
      order = 0;
    }
}


  
