#define brkpin 10
<<<<<<< HEAD
#define tachpin 3
<<<<<<< HEAD
//</define the IO pins for easier Refrencce>

//<Init Motor Drivers>
// Creadit where cedit is due - This was copied and pasted from the Sparkfun Hookup Guide
//https://github.com/sparkfun/SparkFun_TB6612FNG_Arduino_Library/blob/master/examples/MotorTestRun/MotorTestRun.ino
=======
>>>>>>> origin/master
=======
#define tachpin 2
>>>>>>> parent of b60f1d8... Added Motor Driver to fritzing, commented code, +

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
<<<<<<< HEAD
<<<<<<< HEAD
  //<turn on LED 13 to signify init>
=======
>>>>>>> parent of b60f1d8... Added Motor Driver to fritzing, commented code, +
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
<<<<<<< HEAD
=======
  //<turn on 13 to show init>
  pinMode(13, OUTPUT); 
  digitalWrite(13, HIGH);
  //</turn on 13 to show init>
  pinMode(brkpin, OUTPUT);
  pinMode(tachpin, INPUT);
  attachInterrupt(0, tachLog, RISING);
  Serial.begin(9600); //start a serial port for debugging and loading track length
  Serial.print("Enter track Length");
}

void loop() {
  motor2.drive(255, 1000);
  Serial.println(spd);
  if (millis() > 5000) {
    stp();
  }
=======
>>>>>>> parent of b60f1d8... Added Motor Driver to fritzing, commented code, +
  tachCount();
}

void stp() {
  digitalWrite(brkpin, HIGH);
<<<<<<< HEAD
  motor2.brake();
>>>>>>> origin/master
=======
>>>>>>> parent of b60f1d8... Added Motor Driver to fritzing, commented code, +
}

void tachLog() {
  thisTach = millis();
}

void tachCount() {
<<<<<<< HEAD
<<<<<<< HEAD
  //<local variables>
  //I use local variables so that if tachcount is called in the middle of the function it wont make the maths go poof.
  unsigned long lclLast = lastTach;
  unsigned long lclThis = thisTach;
  //</local Variables>
  spd = 1822 / (lclThis - lclLast) * 100; //cm/ms, //1.82212373908208 = dist, //* 100 is to avoid floating point stuff
  if (!(lclThis == thisTach) {//if tachLog has been called since we started
  tachCount();//do this function again
    //I love recursion
=======
  if (!(thisTach = lastTach)) {
    if (thisTach - lastTach < 65535) {//65535 is the max value if an unsigned int, keeps from overflow problems
      unsigned int timeDif = (thisTach - lastTach);
      spd = 1822 / timeDif; //cm/ms, //1.82212373908208 = dist
    }
    loc += spd * (thisTach - lastTach);
    lastTach = thisTach;
>>>>>>> origin/master
=======
  if (thisTach - lastTach < 65535) {//65535 is the max value if an unsigned int
    unsigned int timeDif = (thisTach - lastTach);
    spd = 1822 / timeDif * 100; //cm/ms, //1.82212373908208 = dist, //* 100 is to avoid floating point stuff
>>>>>>> parent of b60f1d8... Added Motor Driver to fritzing, commented code, +
  }
  lastTach = thisTach;
  thisTach = 0;
}
