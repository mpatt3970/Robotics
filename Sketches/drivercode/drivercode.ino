



int ENA = 3;

int IN1 = 2;

int IN2 = 4;


int ENB = 6;

int IN3 = 7;

int IN4 = 8;


void setup ()

{

pinMode (ENA, OUTPUT);

pinMode (IN1, OUTPUT);

pinMode (IN2, OUTPUT);

pinMode (ENB, OUTPUT);

pinMode (IN3, OUTPUT);

pinMode (IN4, OUTPUT);

Serial.begin (9600);


}


void test_motor_1 ()

{

Serial.print ("Motor 1 Speed:");

for (int i = 0; i < 256; i++)

{

digitalWrite (IN1, HIGH);

digitalWrite (IN2, LOW);

analogWrite (ENA, i);

delay (50);

Serial.println(i);

}


delay (3000);

digitalWrite (IN1, LOW);

for (int i = 0; i < 256; i++)

{

digitalWrite (IN1, LOW);

digitalWrite (IN2, HIGH);

analogWrite (ENA, i);

delay (50);

Serial.println(i);

}


delay (3000);

digitalWrite (IN2, LOW);

}


void test_motor_2 ()

{

Serial.print ("Motor 2 Speed");

for (int i = 0; i < 256; i++)

{

digitalWrite (IN3, HIGH);

digitalWrite (IN4, LOW);

analogWrite (ENB, i);

delay (50);

Serial.println(i);

}


delay (3000);

digitalWrite (IN3, LOW);

for (int i = 0; i < 256; i++)

{

digitalWrite (IN3, LOW);

digitalWrite (IN4, HIGH);

analogWrite (ENB, i);

delay (50);

Serial.println(i);

}


delay (3000);

digitalWrite (IN4, LOW);

}


void loop()

{

test_motor_1();

test_motor_2();

}

