
int motor1pin1 = 2;
int motor1pin2 = 3;

int motor2pin1 = 4;
int motor2pin2 = 5;

void setup() {
  // put your setup code here, to run once:
  pinMode(motor1pin1, OUTPUT);
  pinMode(motor1pin2, OUTPUT);
  pinMode(motor2pin1,   OUTPUT);
  pinMode(motor2pin2, OUTPUT);

  //(Optional)
  pinMode(9,   OUTPUT); 
  pinMode(10, OUTPUT);
  //(Optional)
  Serial.begin(9600); 
}

void loop()  
{  

//Controlling speed (0   = off and 255 = max speed):     
  //(Optional)
analogWrite(9, 255); //ENA   pin
analogWrite(10, 255); //ENB pin

drive('f');  // Move forward

int a;
a=analogRead(A1); 
Serial.println(a); 

while(a < 100){
  delay(100);
  drive('s');
}
} 

void drive(char input) {
  switch (input) {
    case 'f': // Move forward
      digitalWrite(motor1pin1, HIGH);
      digitalWrite(motor1pin2, LOW);
      digitalWrite(motor2pin1, LOW);
      digitalWrite(motor2pin2, HIGH);
      break;
      
    case 'b': // Move backward
      digitalWrite(motor1pin1, LOW);
      digitalWrite(motor1pin2, HIGH);
      digitalWrite(motor2pin1, HIGH);
      digitalWrite(motor2pin2, LOW);
      break;
      
    case 'r': // Turn right
      digitalWrite(motor1pin1, LOW);
      digitalWrite(motor1pin2, HIGH);
      digitalWrite(motor2pin1, LOW);
      digitalWrite(motor2pin2, HIGH);
      break;
      
    case 'l': // Turn left
      digitalWrite(motor1pin1, HIGH);
      digitalWrite(motor1pin2, LOW);
      digitalWrite(motor2pin1, HIGH);
      digitalWrite(motor2pin2, LOW);
      break;
      
    case 's': // Turn left
      digitalWrite(motor1pin1, LOW);
      digitalWrite(motor1pin2, LOW);
      digitalWrite(motor2pin1, LOW);
      digitalWrite(motor2pin2, LOW);
      break;
  }
}



