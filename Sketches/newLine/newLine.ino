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
const int MODDERATE = 200;
const int FASTER = 300;

//set servo speeds
const int STOP = 90;
const int RIGHT = 110;
const int LEFT = 70;

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

int checkCloseState() {
  //This reads the near sensor, then checks it against currently readable values
  //returns a int representing a field on the color wheel
  // - and more importantly, the angle of the arm or the stop state
  int  near = analogRead(closeSensor);
  int state = -1;
  if (near < 100) {
    state = 0;
  } else if (near < 200) {
    state = 1;
  } else if (near < 300) {
    state = 2;
  } else if (near < 400) {
    state = 3;
  } else if (near < 500) {
    state = 4;
  } else if (near < 600) {
    state = 5;
  } else if (near < 700) {
    state = 6;
  } else if (near < 800) {
    state = 7;
  } else if (near < 900) {
    state = 8;
  }
  return state;
}

boolean sweepLeftDFS() {
  //enter a loop that moves left until the stop state is reached or the line is found
  while (!checkFound() && checkCloseState() != 0) { //0 here represents the stop state
    rotator.write(LEFT); //move left 
    delay(25);
  }
  //stop the servo
  rotator.write(STOP); //or whatever the stop value is
  //if it was successful return true, so that lost doesn't call the other sweep function
  if (checkFound()) {
    //not setting the leftFirst value here because it will be better to read the state to know the direction
    return true;
  } else {
     //move the servo off the stop state
     servo.write(RIGHT); //move right for a little bit
     delay(100); //hopefully long enough to get off the stop state
     return false;
  }
}

boolean sweepRightDFS() {
  //enter a loop that moves left until the stop state is reached or the line is found
  while (!checkFound() && checkCloseState() != 8) { //8 here represents the stop state
    rotator.write(RIGHT); //move right
    //update more frequently to avoid overshooting the line
    delay(25);
  }
  //stop the servo
  rotator.write(STOP); //or whatever the stop value is
  //if it was successful return true, so that lost doesn't call the other sweep function
  if (checkFound()) {
    //not setting the leftFirst value here because it will be better to read the state to know the direction
    return true;
  } else {
    //move the servo off the stop state so that the 
     servo.write(LEFT); //move left for a little bit
     delay(100); //hopefully long enough to get off the stop state
     return false;
  }
}

void straightenUp() {
  if (checkFound()) {
    //only act if the line was found
    int state = checkCloseState();
    //based on this state we can determine how much to turn, roughly
    //something like oldRight() for a duration of (time_for_90/angle_of_state)
    //and straighten the servo arm
    //leftFirst should also be updated based on the direction of the turn
  } else {
    //if the line was not found, damage control mode
    //set the servo to the middle, and back up a little, maybe a random amount to try to avoid an endless loop
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
  //check if the line is found and if the near sensor is in middle
  if (checkFound() && checkCloseState() == 4) { //4 here represents the middle
    //move forward at speed of faster
    digitalWrite(speedRight, FASTER);
    digitalWrite(speedLeft, FASTER);
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
