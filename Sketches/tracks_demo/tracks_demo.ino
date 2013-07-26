//pin definitions
int EnL = 3; //ENA
int ForL = 2; //IN1
int RevL = 4; //IN2
int EnR = 6; //ENB
int ForR = 7; //IN3
int RevR = 8; //IN4

//motor speed
int spd = 150; 

void setup ()

{
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

delay(2000);

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

delay (2000);

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
  delay (2000);
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
  delay (2000);
  digitalWrite (ForR, LOW);
  digitalWrite (RevL, LOW);
}

void loop()

{

forward();

delay(2000);

backward();

delay (2000);

right();

delay (2000);

left();

delay (2000);

}

