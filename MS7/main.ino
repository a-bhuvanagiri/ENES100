#include "Enes100.h"
#include "nav_functions.h"
#include "mission_sequence.h"
#include "log.h"

//delete later
#include <Arduino.h>
#include <Servo.h>
#include "Enes100.h"




float east = 1.54;


int signalPin = 8;
int hallPin = A0;

bool servoLowered = false;
bool servoRaised = false;

// Motor pins
int motor1pin1 = 2;
int motor1pin2 = 3;
int motor2pin1 = 4;
int motor2pin2 = 5;

bool missionDone = false;


void setup() {
  Enes100.begin("Sabine's Carpenters", DATA, 333, 1120, 12, 13);
  Enes100.print("Working as intended.");
  
  //servo.attach(11);
  //servo.write(90);
  
  pinMode(motor1pin1, OUTPUT);
  pinMode(motor1pin2, OUTPUT);
  pinMode(motor2pin1, OUTPUT);
  pinMode(motor2pin2, OUTPUT);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(9, OUTPUT); 
  pinMode(10, OUTPUT); 

  Serial.begin(9600); 
}

void loop() {

  Enes100.println("Running in:");
  delay(1000);
  Enes100.println("3");
  delay(1000);
  Enes100.println("2");
  delay(1000);
  Enes100.println("1");
  /*
  delay(1000);
  drive('f');
  delay(2000);
  drive('s');
  delay(5000);
  servo.attach(11);
  servo.write(90);
  delay(1000);
  servo.write(95);
  delay(4000);
  servo.write(90);
  delay(4000);
  servo.write(87);
  delay(8000);
  servo.write(90);
  delay(100000);
  */




  //this is to test at the beginning of the next openlab section

  Enes100.println("Running the start sequence.");
  if (!missionDone) {
    float y = Enes100.getY();

    if (y > 1.2) {
      Enes100.println("In position A.");
      alignToAngle(south);
      moveToPylon();
      runMission();
      missionDone = true;
    } else if (y < 0.8 && y > 0.0) {
      Enes100.println("In position B.");
      alignToAngle(north);
      moveToPylon();
      runMission();
      missionDone = true;
    }
  }

  if (missionDone) {
    drive('b');
    delay(4500);
    drive('s');
  }

  servo.write(95); 
  delay(2000);
  servo.write(90);
  servo.detach();

  Enes100.println("Start sequence completed.");
  
  faceEast();
  Enes100.println("I am now facing east.");

  Enes100.println("Starting obstacle navigation.");
  obstacleNavigation();
  Enes100.println("Finished obstacle navigation.");

  Enes100.println("Starting the log sequence.");
  logSequence();
  

  Enes100.println("Now going to face east.");
  faceEast();

  drive('f');
  Enes100.println("Driving forward to freedom.");
  delay(20000);
  while(Enes100.getX()<3.9){
    Enes100.println("Trying again.");
    drive('s');
    delay(1000);
    drive('f');
    delay(5000);
  }


  drive('s');
  Enes100.println("They didn't believe in us.");
  delay(2000);
  Enes100.println("But we did it anyway.");
  delay(100000);

}
