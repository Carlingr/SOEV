#define brkpin 10
#define tachpin 3

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
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  pinMode(brkpin, OUTPUT);
  pinMode(tachpin, INPUT);
  attachInterrupt(0, tachLog, RISING);
  Serial.print("Enter track Length");
}

void loop() {
  motor2.drive(255, 1000);
  Serial.println(spd);
  if (millis() > 5000) {
    stp();
  }
  tachCount();
}

void stp() {
  digitalWrite(brkpin, HIGH);
  motor2.brake();
}

void tachLog() {
  thisTach = millis();
}

void tachCount() {
  if (!(thisTach = lastTach)) {
    if (thisTach - lastTach < 65535) {//65535 is the max value if an unsigned int, keeps from overflow problems
      unsigned int timeDif = (thisTach - lastTach);
      spd = 1822 / timeDif; //cm/ms, //1.82212373908208 = dist
    }
    loc += spd * (thisTach - lastTach);
    lastTach = thisTach;
  }
}
