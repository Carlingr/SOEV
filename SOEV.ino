#include <Servo.h>

Servo myservo;

float trk = 0;
float loc = 0;


void setup() {
  digitalWrite(13, HIGH);
  myservo.attach(9);

  // put your setup code here, to run once:

}

void loop() {
  myservo.write(0);
  delay(2000);
  myservo.write(180);
  delay(2000);
  // put your main code here, to run repeatedly:

}
