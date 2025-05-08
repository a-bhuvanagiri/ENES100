
  #ifndef LOG_H
  #define LOG_H

  #include <Arduino.h>
  #include "Enes100.h"

  void logSequence(){
    if(Enes100.getY() > 1.2){
      Enes100.println("Above Log");
      drive('r');
      delay(ninetyTime);
      drive('f');
      delay(14000);
      drive('l');
      delay(ninetyTime);
      drive('f');
    }
    else if(Enes100.getY() > 0.8){
      Enes100.println("Slightly Above Log");
      drive('r');
      delay(ninetyTime);
      drive('f');
      delay(6000);
      drive('l');
      delay(ninetyTime);
      drive('f');
    } 
    else {
      Enes100.println("In Line With Log");
      drive('f');
    }
  }

  #endif
