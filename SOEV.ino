//<define the IO pins for easier Refrencce>
// #define is good for IO pins because #define and IO pins are both hard wiring
#define brkpin 10
#define tachpin 3
//</define the IO pins for easier Refrencce>

//<Init Motor Drivers>
// Creadit where cedit is due - This was copied and pasted from the Sparkfun Hookup Guide
//https://github.com/sparkfun/SparkFun_TB6612FNG_Arduino_Library/blob/master/examples/MotorTestRun/MotorTestRun.ino

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
Motor prop = Motor(AIN1, AIN2, PWMA, offsetA, STBY);
Motor turn = Motor(BIN1, BIN2, PWMB, offsetB, STBY);
//</Init Motor Drivers>

//<Init Variables>
//Floats are evil due to slow computation speeds. AVOID!
unsigned long trk; //use cm
unsigned long loc; //use cm
const unsigned long decel=0; //use cm*(cm/ms)
unsigned long spd; //speed in cm/ms
//<used by tachometer>
//global becuase they are used in tachLog() and tachCount()
unsigned long lastTach;
unsigned long thisTach;
//<used by tachCount Function>
//</Init Variables>

void setup() {
  //<turn on LED 13 to signify init>
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  //</turn on LED 13 to signify init>
  //<pin Modes>
  pinMode(brkpin, OUTPUT);
  pinMode(tachpin, INPUT);
  //</pin Modes>
  attachInterrupt(0, tachLog, RISING); //if the tachpin is pulled high (the light shines through the holes in the wheels)
  //Interrupt is used becuase the code may miss it if it is only called during loop
  prop.drive(255); //turn the motor on
  Serial.println("trk,loc,spd,lastTach,thisTach"); //tells the order of the variables when printed
}

void loop() {
  //<log all the variables for debuging>
  Serial.print(trk);
  Serial.print(",");
  Serial.print(loc);
  Serial.print(",");
  Serial.print(spd);
  Serial.print(",");
  Serial.print(lastTach);
  Serial.print(",");
  Serial.println(thisTach);
  //</log all the variable for debugging>
  tachCount();// do the tach maths
  if (trk - loc <= decel * spd) { //if we are running out of track
    prop.brake(); //brake the motors
    digitalWrite(brkpin, HIGH); //fire the solenoids
  }
}

void tachLog() {//interrupt for tachometer
  lastTach = thisTach;//move thisTach into lastTach to make room for the new reading
  thisTach = millis();// get the new reading
}

void tachCount() {
  //<local variables>
  //I use local variables so that if tachcount is called in the middle of the function it wont make the maths go poof.
  unsigned long lclLast = lastTach;
  unsigned long lclThis = thisTach;
  //</local Variables>
  spd = 1822 / (lclThis - lclLast) * 100; //cm/ms, //1.82212373908208 = dist, //* 100 is to avoid floating point stuff
  if (!(lclThis == thisTach)) {//if tachLog has been called since we started
    tachCount();//do this function again
    //I love recursion
  }
}
