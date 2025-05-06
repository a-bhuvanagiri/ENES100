#ifndef NAV_MISSION_H
#define NAV_MISSION_H

#include <Arduino.h>
#include <Servo.h>
#include "Enes100.h"

// Orientation constants
const float north = 3.14;
const float east  = 1.57;
const float south = 0;
const float west  = -1.57;

// Shared motor and pin definitions
extern int motor1pin1;
extern int motor1pin2;
extern int motor2pin1;
extern int motor2pin2;
extern int signalPin;
extern int hallPin;
extern bool servoLowered;
extern bool servoRaised;
extern Servo servo;
extern float ninetyTime;

// -------------------- Driving --------------------
void drive(char input) {
  analogWrite(9, 255);
  analogWrite(10, 255);

  switch (input) {
    case 'f':
      digitalWrite(motor1pin1, HIGH); digitalWrite(motor1pin2, LOW);
      digitalWrite(motor2pin1, LOW);  digitalWrite(motor2pin2, HIGH);
      break;
    case 'b':
      digitalWrite(motor1pin1, LOW);  digitalWrite(motor1pin2, HIGH);
      digitalWrite(motor2pin1, HIGH); digitalWrite(motor2pin2, LOW);
      break;
    case 'r':
      digitalWrite(motor1pin1, LOW);  digitalWrite(motor1pin2, HIGH);
      digitalWrite(motor2pin1, LOW);  digitalWrite(motor2pin2, HIGH);
      break;
    case 'l':
      digitalWrite(motor1pin1, HIGH); digitalWrite(motor1pin2, LOW);
      digitalWrite(motor2pin1, HIGH); digitalWrite(motor2pin2, LOW);
      break;
    case 's':
      digitalWrite(motor1pin1, LOW);  digitalWrite(motor1pin2, LOW);
      digitalWrite(motor2pin1, LOW);  digitalWrite(motor2pin2, LOW);
      break;
  }
}

// -------------------- Orientation --------------------
void alignToAngle(float target) {
  while (fabs(Enes100.getTheta() - target) > 0.2) {
    drive((target == north) ? 'l' : 'r');
  }

  Enes100.println("Refining alignment...");
  while (fabs(Enes100.getTheta() - target) > 0.05) {
    drive((target == north) ? 'l' : 'r');
    delay(30);
    drive('s');
    delay(150);
  }
  Enes100.println("Facing correct direction.");
}

// -------------------- Approach IR Beacon --------------------
void moveToPylon() {
  drive('f');
  int a = analogRead(A1);
  while (a > 30) {
    drive('f');
    delay(500);
    a = analogRead(A1);
  }

  drive('s');
  delay(300);
  Enes100.println("Pylon reached.");
}

// -------------------- Mission Sequence --------------------
void runMission() {
  if (!servoLowered) {
    Enes100.println("Lowering servo...");
    servo.write(95); delay(4000);
    servoLowered = true;
  }

  unsigned long highTime = pulseIn(signalPin, HIGH, 500000); 
  unsigned long lowTime  = pulseIn(signalPin, LOW, 500000);

  delay(2000);
  Enes100.println("Reading duty cycle...");

  if (highTime > 0 && lowTime > 0) {
    float dutyCycle = (float)highTime / (highTime + lowTime) * 100.0;
    int roundedDuty;

    if (dutyCycle < 20)       roundedDuty = 10;
    else if (dutyCycle < 40)  roundedDuty = 30;
    else if (dutyCycle < 60)  roundedDuty = 50;
    else if (dutyCycle < 80)  roundedDuty = 70;
    else                      roundedDuty = 90;

    Enes100.println(100 - roundedDuty);
    Enes100.print("Duty Cycle: ");
    Enes100.print(100 - dutyCycle);
    Enes100.print("%, Rounded Duty: ");
    Enes100.println(100 - roundedDuty);
  }

  delay(4000);
  Enes100.println("Reading magnetic field...");
  float sensorValue = analogRead(hallPin);
  Enes100.print("Magnetic Sensor Value: ");
  Enes100.println(sensorValue);
  Enes100.println(sensorValue < 1000 ? "Magnetic field detected!" : "No magnetic field detected.");

  Enes100.println("Raising servo...");
  servo.write(80); delay(8000);
  servoRaised = true;

  Enes100.println("Mission tasks complete. Retreating...");
  drive('b'); delay(5000);
  drive((Enes100.getY() > 1.0) ? 'l' : 'r'); delay(ninetyTime);
  drive('f');
}

#endif
