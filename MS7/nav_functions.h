#ifndef NAV_FUNCTIONS_H
#define NAV_FUNCTIONS_H

#include <Arduino.h>
#include "Enes100.h"

// Extern so motor pins can be accessed from main.ino
extern int motor1pin1;
extern int motor1pin2;
extern int motor2pin1;
extern int motor2pin2;

int trigPin = 6;    
int echoPin = 7; 


//for nav
extern float ninetyTime = 5000;

//consider a function to find the average theta value, and then facing east by determing a certain amount of time to run.
float avgTheta(){
  float thetaSum = 0.0;

  for(int i = 0; i < 5; i++){
    if(Enes100.getTheta() != -1){
      thetaSum += Enes100.getTheta();
    } 
    delay(100);
  }
  return thetaSum / 5.0;
}

float avgY() {
  float ySum = 0.0;

  for (int i = 0; i < 5; i++) {
    if(Enes100.getY() != -1){
      ySum += Enes100.getY();
    } 
    delay(100);
  }

  return ySum / 5.0;
}

float avgX() {
  float xSum = 0.0;

  for (int i = 0; i < 5; i++) {
    if(Enes100.getX() != -1){
      xSum += Enes100.getX();
    } 
    delay(100);
  }

  return xSum / 5.0;
}

float fullRotationTime = 18.1;

boolean obstaclePresent(){
  float duration;
  float distance;

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = (duration*.0343)/2;
  
  if(distance < 10){
    return true;
  }
  else{
    return false;
  }
}

void drive(char input) {
  analogWrite(9, 255);
  analogWrite(10, 255);

  switch (input) {
    case 'f':
      digitalWrite(motor1pin1, HIGH);
      digitalWrite(motor1pin2, LOW);
      digitalWrite(motor2pin1, LOW);
      digitalWrite(motor2pin2, HIGH);
      break;
    case 'b':
      digitalWrite(motor1pin1, LOW);
      digitalWrite(motor1pin2, HIGH);
      digitalWrite(motor2pin1, HIGH);
      digitalWrite(motor2pin2, LOW);
      break;
    case 'r':
      digitalWrite(motor1pin1, LOW);
      digitalWrite(motor1pin2, HIGH);
      digitalWrite(motor2pin1, LOW);
      digitalWrite(motor2pin2, HIGH);
      break;
    case 'l':
      digitalWrite(motor1pin1, HIGH);
      digitalWrite(motor1pin2, LOW);
      digitalWrite(motor2pin1, HIGH);
      digitalWrite(motor2pin2, LOW);
      break;
    case 's':
      digitalWrite(motor1pin1, LOW);
      digitalWrite(motor1pin2, LOW);
      digitalWrite(motor2pin1, LOW);
      digitalWrite(motor2pin2, LOW);
      break;
  }
}


void faceEast() {


  if(Enes100.getTheta() > 1.55){
      while(Enes100.getTheta() > 1.56){
        drive('r');
        delay(30);
        drive('s');
        delay(100);
      }
  }
  else if(Enes100.getTheta() < 1.55){
    while(Enes100.getTheta() < 1.54){
        drive('l');
        delay(30);
        drive('s');
        delay(100);
      }
  }
}


//below are functions for obstacle navigation
void runCenter1stRow(){
  while(!obstaclePresent()){
    drive('f');
    delay(100);
    if(Enes100.getX() > 1.5){
      Enes100.println("No obstacle detected in first row.");
      return;
    }
  }
  drive('l');
  delay(ninetyTime);
  drive('f');
  delay(6000);
  drive('r');
  delay(ninetyTime);
  while(!obstaclePresent()){
    drive('f');
    delay(100);
    if(Enes100.getX() > 1.5){
      Enes100.println("Only center obstacle detected in first row.");
      return;
    }
  }
  drive('r');
  delay(ninetyTime);
  drive('f');
  delay(14000);
  drive('l');
  delay(ninetyTime);
  drive('f');
  while(Enes100.getX() < 1.5){
    drive('f');
    delay(100);
  }
  Enes100.println("Both obstacles detected in first row.");
}

//this runs if the OTV does not encounter an obstacle in the first row, going straight to the second
void runCenter2ndRow(){
  while(!obstaclePresent()){
    drive('f');
    delay(100);
    if(Enes100.getX() > 2.5){
      Enes100.println("No obstacle detected in second row.");
      return;
    }
  }
  drive('l');
  delay(ninetyTime);
  drive('f');
  delay(6000);
  drive('r');
  delay(ninetyTime);
  while(!obstaclePresent()){
    drive('f');
    delay(100);
    if(Enes100.getX() > 2.5){
      Enes100.println("Only center obstacle detected in second row.");
      return;
    }
  }
  drive('r');
  delay(ninetyTime);
  drive('f');
  delay(12000);
  drive('l');
  delay(ninetyTime);
  drive('f');
  while(Enes100.getX() < 2.8){
    delay(100);
    drive('f');
  }
  Enes100.println("Both obstacles detected in second row.");
}

//this runs if the OTV is in the top spot when entering the second row
void runTop2ndRow(){
  while(!obstaclePresent()){
    drive('f');
    delay(100);
    if(Enes100.getX() > 2.8){
      Enes100.println("No obstacle detected in the top spot of the second row.");
      return;
    }
  }
  drive('r');
  delay(ninetyTime);
  drive('f');
  delay(8000);
  drive('l');
  delay(ninetyTime);
  while(Enes100.getX() < 2.5){
    delay(100);
    drive('f');
  }
  Enes100.println("Obstacle detected in the top spot of the second row.");
}


//this is the only one that needs to be run in the overall code
void obstacleNavigation(){
  //first function, will end when OTV reaches second row
  runCenter1stRow();
  
  Enes100.println("In the second row, x > 1.5");

  //checks all of the possible places that the OTV can be in when reaching the 2nd row
  if(avgY() < 1.2 && avgY() > 0.8){
    Enes100.println("Running the runCenter2ndRow function");
    runCenter2ndRow();
  }
  else if(avgY() > 1.2){
    Enes100.println("Running the runTop2ndRow function");
    runTop2ndRow();
  }
  else{
    Enes100.println("I know there will be no obstacle in the 2nd row.");
    drive('f');
    while(Enes100.getX() < 2.5){
      delay(100);
      drive('f');
    }
  }
  
  Enes100.println("In the open zone, x > 2.5");
}



#endif
