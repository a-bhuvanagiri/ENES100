#include <Servo.h>
Servo servo;


void setup() {
  // put your setup code here, to run once:
  
  servo.attach(11);
}

void loop() {
  // put your main code here, to run repeatedly:

  servo.write(95);
  delay(3000);
  servo.write(90);
  delay(3000);
  servo.write(85);
  delay(3000);



}

/*
>90: Clockwise
90: stop
<90: Counter Clockwise

*/