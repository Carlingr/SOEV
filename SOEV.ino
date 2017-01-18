#define brkpin 10
#define tachpin 2

// This is the library for the TB6612 that contains the class Motor and all the
// functions
#include <SparkFun_TB6612.h>

// Pins for all inputs, keep in mind the PWM defines must be on PWM pins
// the default pins listed are the ones used on the Redbot (ROB-12097) with
// the exception of STBY which the Redbot controls with a physical switch
#define AIN1 2
#define BIN1 7
#define AIN2 4
#define BIN2 8
#define PWMA 5
#define PWMB 6
#define STBY 9

// these constants are used to allow you to make your motor configuration
// line up with function names like forward.  Value can be 1 or -1
const int offsetA = 1;
const int offsetB = 1;

// Initializing motors.  The library will allow you to initialize as many
// motors as you have memory for.  If you are using functions like forward
// that take 2 motors as arguements you can either write new functions or
// call the function more than once.
Motor motor1 = Motor(AIN1, AIN2, PWMA, offsetA, STBY);
Motor motor2 = Motor(BIN1, BIN2, PWMB, offsetB, STBY);

unsigned long trk; //use cm
unsigned long loc; //use cm
unsigned long decel; //use
float spd; //speed in cm/second
unsigned long lastTach;
unsigned long thisTach;

void setup() {
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  pinMode(brkpin, OUTPUT);
  pinMode(tachpin, INPUT);
  attachInterrupt(0, tachLog, RISING);
  Serial.print("Enter track Length");
}

void loop() {
  //Use of the forward function, which takes as arguements two motors
  //and optionally a speed.  If a negative number is used for speed
  //it will go backwards
  forward(motor1, motor2, 150);
  delay(1000);

  //Use of the back function, which takes as arguments two motors
  //and optionally a speed.  Either a positive number or a negative
  //number for speed will cause it to go backwards
  back(motor1, motor2, -150);
  delay(1000);

  if (trk - loc <= decel * spd) {
    stp();
  }
  tachCount();
}

void stp() {
  digitalWrite(brkpin, HIGH);
}

void tachLog() {
  thisTach = millis();
}

void tachCount() {
  if (thisTach - lastTach < 65535) {//65535 is the max value if an unsigned int
    unsigned int timeDif = (thisTach - lastTach);
    spd = 1822 / timeDif * 100; //cm/ms, //1.82212373908208 = dist, //* 100 is to avoid floating point stuff
  }
  lastTach = thisTach;
  thisTach = 0;
}
