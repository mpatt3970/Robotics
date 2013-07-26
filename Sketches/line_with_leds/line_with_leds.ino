//pin definitions
int AN0 = A0;
int AN1 = A1;
int EnL = 3; //ENA
int ForL = 2; //IN1
int RevL = 4; //IN2
int EnR = 6; //ENB
int ForR = 7; //IN3
int RevR = 8; //IN4

//motor speed
int spd = 150; 
//threshold for light
int THRES = 750;
//time for movning
int TIME = 2000;
void setup()
 {
pinMode (AN0, INPUT);
pinMode (AN1, INPUT);
pinMode (EnL, OUTPUT);
pinMode (ForL, OUTPUT);
pinMode (RevL, OUTPUT);
pinMode (EnR, OUTPUT);
pinMode (ForR, OUTPUT);
pinMode (RevR, OUTPUT);
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

void follow() {
  if (AN0 > THRES && AN1 > THRES)
  {
    forward();
  }
  else {
    find();
  }
}

void find()
{
  if ( AN0 > THRES && AN1 < THRES)
  {
    right();
  }
  else if (AN0 < THRES && AN1 > THRES)
  {
    left();
  }
}

void loop() {
  forward();
  backward();
  left();
  right();
}
