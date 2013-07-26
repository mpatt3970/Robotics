//pin definitions
int AnL = A0;
int AnR = A1;
int EnL = 3; //ENA
int ForL = 2; //IN1
int RevL = 4; //IN2
int EnR = 6; //ENB
int ForR = 7; //IN3
int RevR = 8; //IN4
int LedR = 5; //right light
int LedL = 12; //left light

//motor speed
int spd = 150; 
//threshold for light
int THRES = 750;
//time for movning
int TIME = 200;
void setup()
 {
pinMode (AnL, INPUT);
pinMode (AnR, INPUT);
pinMode (EnL, OUTPUT);
pinMode (ForL, OUTPUT);
pinMode (RevL, OUTPUT);
pinMode (EnR, OUTPUT);
pinMode (ForR, OUTPUT);
pinMode (RevR, OUTPUT);
pinMode (LedR, OUTPUT);
pinMode (LedL, OUTPUT);
}

void forward ()
{
digitalWrite (ForR, HIGH);

digitalWrite (RevR, LOW);

analogWrite (EnR, spd);

digitalWrite (ForL, HIGH);

digitalWrite (RevL, LOW);

analogWrite (EnL, spd);

delay(TIME);

digitalWrite (ForL, LOW);

digitalWrite (ForR, LOW);

}

void backward ()
{
digitalWrite (ForL, LOW);

digitalWrite (RevL, HIGH);

analogWrite (EnL, spd);

digitalWrite (ForR, LOW);

digitalWrite (RevR, HIGH);

analogWrite (EnR, spd);

delay (TIME);

digitalWrite (RevL, LOW);

digitalWrite (RevR, LOW);
}

void right()
{
  digitalWrite (ForR, LOW);
  digitalWrite (RevR, HIGH);
  analogWrite (EnR, spd);
  digitalWrite (ForL, HIGH);
  digitalWrite (RevL, LOW);
  analogWrite (EnL, spd);
  delay (TIME);
  digitalWrite (RevR, LOW);
  digitalWrite (ForL, LOW);
}

void left()
{
  digitalWrite (ForL, LOW);
  digitalWrite (RevL, HIGH);
  analogWrite (EnL, spd);
  digitalWrite (ForR, HIGH);
  digitalWrite (RevR, LOW);
  analogWrite (EnR, spd);
  delay (TIME);
  digitalWrite (ForR, LOW);
  digitalWrite (RevL, LOW);
}
void flash(int led)
{
  digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);               // wait for a second
  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
}
void follow() {
  if (analogRead(AnL) > THRES && analogRead(AnR) > THRES)
  {
    digitalWrite(LedR, HIGH);
    digitalWrite(LedL, HIGH);
    forward();
  }
  else {
    digitalWrite(LedR, LOW);
    digitalWrite(LedL, LOW);
    find();
  }
}

void find()
{
  l_r = analogRead(AnR);
  l_l = analogRead(AnL);
  if ( l_l > THRES && l_r < THRES)
  {
    digitalWrite(LedR, HIGH);
    digitalWrite(LedL, LOW);
    right();
  }
  else if (l_l < THRES && l_r > THRES)
  {
    digitalWrite(LedR, LOW);
    digitalWrite(LedL, HIGH);
    left();
  }
  else if (l_l < THRES && l_r < THRES)
  {
    digitalWrite(LedR, LOW);
    digitalWrite(LedL, LOW);
    backward();
  }
}

void loop() {
  follow();
}
