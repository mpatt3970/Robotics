#include <Servo.h>
//declare Servo variable
Servo rotator;
//remember which way it turned last so that chances are that it guesses correctly first
boolean leftFirst = true;
//pin definitions
int closeSensor = A0;
int farSensor = A1;
int speedLeft = 3; //ENA
int forwardL = 2; //IN1
int reverseL = 4; //IN2
int speedRight = 6; //ENB
int forwardR = 7; //IN3
int reverseR = 8; //IN4
int servoPin = 9; //????

//required for old functions
const int THRES = 700;
const int TIME = 2000;
const int SPEED = 150;

//set some values for speeds, fine tune later
const int SLOWER = 100;
const int MODERATE = 200;
const int FASTER = 300;

const int TURN_90_TIME = 2000;

void setup() {
  //declares the pins
  pinMode (closeSensor, INPUT);
  pinMode (farSensor, INPUT);
  pinMode (speedLeft, OUTPUT);
  pinMode (forwardL, OUTPUT);
  pinMode (reverseL, OUTPUT);
  pinMode (speedRight, OUTPUT);
  pinMode (forwardR, OUTPUT);
  pinMode (reverseR, OUTPUT);
  rotator.attach(servoPin);
}

//left these functions for reference

void oldForward () {
  //one step forward
  digitalWrite (forwardR, HIGH);
  digitalWrite (reverseR, LOW);
  analogWrite (speedRight, SPEED);
  digitalWrite (forwardL, HIGH);
  digitalWrite (reverseL, LOW);
  analogWrite (speedLeft, SPEED);
  delay(TIME);
  digitalWrite (forwardL, LOW);
  digitalWrite (forwardR, LOW);
}

void oldBackward () {
  //one step back
  digitalWrite (forwardL, LOW);
  digitalWrite (reverseL, HIGH);
  analogWrite (speedLeft, SPEED);
  digitalWrite (forwardR, LOW);
  digitalWrite (reverseR, HIGH);
  analogWrite (speedRight, SPEED);
  delay (TIME);
  digitalWrite (reverseL, LOW);
  digitalWrite (reverseR, LOW);
}

void oldRight() {
  //90 degree turn
  digitalWrite (forwardR, LOW);
  digitalWrite (reverseR, HIGH);
  analogWrite (speedRight, SPEED);
  digitalWrite (forwardL, HIGH);
  digitalWrite (reverseL, LOW);
  analogWrite (speedLeft, SPEED);
  delay (TIME);
  digitalWrite (reverseR, LOW);
  digitalWrite (forwardL, LOW);
}

void oldLeft() {
  //90 degree turn
  digitalWrite (forwardL, LOW);
  digitalWrite (reverseL, HIGH);
  analogWrite (speedLeft, SPEED);
  digitalWrite (forwardR, HIGH);
  digitalWrite (reverseR, LOW);
  analogWrite (speedRight, SPEED);
  delay (TIME);
  digitalWrite (forwardR, LOW);
  digitalWrite (reverseL, LOW);
}

void oldFollow() {
  int close = analogRead(closeSensor);
  int far = analogRead(farSensor);
  if (close > THRES && far > THRES) {
    oldForward();
  }
  else {
    oldFind();
  }
}

void oldFind() {
  int l_r = analogRead(farSensor);
  int l_l = analogRead(closeSensor);
  if ( l_l > THRES && l_r < THRES) {
    oldRight();
  }
  else if (l_l < THRES && l_r > THRES) {
    oldLeft();
  }
  else if (l_l < THRES && l_r < THRES) {
    oldBackward();
  }
}

boolean checkFound() {
  int far = analogRead(farSensor);
  boolean out = false;
  if (far > THRES) {
    out = true;
  }
  return out;
}

boolean inMiddle(int angle) {
  //if it's around 90 degrees
  if (angle > 85 && angle < 95) {
    return true;
  }
  return false;
}

boolean sweepLeftDFS() {
  //decrement the servo by 5 degrees and check again
  for (int i = rotator.read() - 5; i > 5 && !checkFound(); i -=5) {
    //move the servo to the next position
    rotator.write(i);
    delay(25);
  }
  //if it was successful return true, so that lost doesn't call the other sweep function
  if (checkFound()) {
    //not setting the leftFirst value here because it will be better to read the state to know the direction
    return true;
  } else {
     return false;
  }
}

boolean sweepRightDFS() {
  //enter a loop that moves left until the stop state is reached or the line is found
  for (int i = rotator.read() + 5; i < 175 && !checkFound(); i += 5) {
    rotator.write(i);
    delay(25);
  }
  //if it was successful return true, so that lost doesn't call the other sweep function
  if (checkFound()) {
    //not setting the leftFirst value here because it will be better to read the state to know the direction
    return true;
  } else {
    return false;
  }
}

void straightenUp() {
  int ratio;
  if (checkFound()) {
    int angle = rotator.read();
    if (angle < 90) {
      //update leftFirst to give it an idea of which to check first next time it is lost
      leftFirst = true;
      ratio = angle/90;
      digitalWrite(reverseL, LOW);
      digitalWrite(forwardR, LOW);
      analogWrite(speedLeft, MODERATE);
      analogWrite(speedRight, MODERATE);
      digitalWrite(forwardL, HIGH);
      digitalWrite(reverseR, HIGH);
    } else {
      leftFirst = false;
      ratio = (angle - 90)/90;
      digitalWrite(reverseR, LOW);
      digitalWrite(forwardL, LOW);
      analogWrite(speedLeft, MODERATE);
      analogWrite(speedRight, MODERATE);
      digitalWrite(forwardR, HIGH);
      digitalWrite(reverseL, HIGH);
    }
    int duration = ratio*TURN_90_TIME;
    rotator.write(90);
    delay(duration);
    
    //only act if the line was found
    //based on this state we can determine how much to turn, roughly
    //something like oldRight() for a duration of (time_for_90/angle_of_state)
    //and straighten the servo arm
    //leftFirst should also be updated based on the direction of the turn
  } else {
    //if the line was not found, damage control mode
    //set the servo to the middle, and back up a little, maybe a random amount to try to avoid an endless loop
    rotator.write(90);
    digitalWrite(forwardL, LOW);
    digitalWrite(forwardR, LOW);
    analogWrite(speedLeft, SLOWER);
    analogWrite(speedRight, SLOWER);
    digitalWrite(reverseL, HIGH);
    digitalWrite(reverseR, HIGH);
    delay(75);
    //and this will exit to the main loop, hopefully the line will be found at this point, and the robot can re-attempt the turn
  }
}

void lost() {
  //if it was found on the left previously start that way
  if (leftFirst) {
    //this is a depth first search, go all the way one way, before checking the other side.
    if (!sweepLeftDFS()) {
      //if it wasn't found on the left, check the right
      sweepRightDFS();
    }
  } else {
    //start right if it was on the right last time
    if (!sweepRightDFS()) {
      sweepLeftDFS();
    }
  }
  straightenUp();
}

void follow() {
  int degrees = rotator.read();
  //check if the line is found and if the servo is centered
  if (checkFound() && inMiddle(degrees)) {
    //move forward at speed of faster
    analogWrite(speedRight, FASTER);
    analogWrite(speedLeft, FASTER);
    digitalWrite(reverseL, LOW);
    digitalWrite(reverseL, LOW);
    digitalWrite(forwardL, HIGH);
    digitalWrite(forwardR, HIGH);
  } else {
    //stop
    digitalWrite(forwardL, LOW);
    digitalWrite(forwardR, LOW);
    lost();
  }
}


void loop() {
  follow();
  delay(100);
}
