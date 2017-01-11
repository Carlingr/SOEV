//#include <Servo.h> //for servo

#define brkpin 2

//Servo brkSrvo; // for servo

float trk;
float loc;
float decel;
float spd;


void setup() {
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  pinMode(brkpin, OUTPUT);
  //brkSrvo.attach(brkpin); //for servo
  Serial.print("Enter track Length");

}

void loop() {
  if (trk - loc <= decel * spd) {
    stp();
  }
}

void stp() {
  digitalWrite(brkpin, HIGH);
  //brkSrvo.write(180)  //for servo
}
