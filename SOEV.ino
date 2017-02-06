//<define the IO pins for easier Refrencce>
// #define is good for IO pins because #define and IO pins are both hard wiring
#define brkpin 10
#define tachpin 3
#define wedge 1822 //distance around wheel between inturrupts, =1.82212373908208, * 100 to avoid floating point
//</define the IO pins for easier Refrencce>

//<Init Motor Drivers>
// Creadit where cedit is due - This was copied and pasted from the Sparkfun Hookup Guide
//https://github.com/sparkfun/SparkFun_TB6612FNG_Arduino_Library/blob/master/examples/MotorTestRun/MotorTestRun.ino11
// This is the library for the TB6612 that contains the class Motor and all the
// functions
#include <SparkFun_TB6612.h>

// Pins for all inputs, keep in mind the PWM defines must be on PWM pins
// the default pins listed are the ones used on the Redbot (ROB-12097) with
// the exception of STBY which the Redbot controls with a physical switch
#define AIN1 11
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
Motor A = Motor(AIN1, AIN2, PWMA, offsetA, STBY);
Motor B = Motor(BIN1, BIN2, PWMB, offsetB, STBY);
//</Init Motor Drivers>

//<Init Variables>
//Floats are evil due to slow computation speeds. AVOID!
unsigned long trk; //use cm
unsigned long loc; //use cm
const unsigned long decel = 0; //use cm*(cm/ms)
unsigned long spd; //speed in cm/ms
//<used by tachometer>
//global becuase they are used in tachLog() and tachCount()
volatile unsigned long lastTach2;
volatile unsigned long thisTach2;
volatile unsigned long lastTach3;
volatile unsigned long thisTach3;
//<used by tachCount Function>
//<Speed used by motors>
byte Aspd = 255; //Start Motor a @ 255
byte Bspd = 255; //Start Motor B @255
//<speed used by motors>
//<used by tachcount, global for efficiency>
unsigned long lclLast;
unsigned long lclThis;
unsigned long spd2;
unsigned long spd3;
//</used by tachcount, global for efficiency>
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
  Serial.begin(300);//Start Serial
  //DO NOT PUT PARENTHESES AFTER ISR!
  attachInterrupt(digitalPinToInterrupt(2), tachLog2, RISING); //if the tachpin is pulled high (the light shines through the holes in the wheels),
  attachInterrupt(digitalPinToInterrupt(3), tachLog3, RISING); //if the tachpin is pulled high (the light shines through the holes in the wheels)
  //Interrupt is used becuase the code may miss it if it is only called during loop
  A.drive(Aspd); //turn the motor on
  B.drive(Bspd); //turn the motor on
  Serial.println("trk,loc,spd,lastTach2,thisTach2,lastTach3,thisTach3,Aspd,Bspd"); //tells the order of the variables when printed
}

void loop() {
  //<Do this every time the code runs>
  //*<log all the variables for debuging>
  Serial.print(trk);
  Serial.print(",");
  Serial.print(loc);
  Serial.print(",");
  Serial.print(spd);
  Serial.print(",");
  Serial.print(lastTach2);
  Serial.print(",");
  Serial.println(thisTach2);
  Serial.print(",");
  Serial.print(lastTach3);
  Serial.print(",");
  Serial.println(thisTach3);
  Serial.print(",");
  Serial.print(Aspd);
  Serial.print(",");
  Serial.println(Bspd);
  //</log all the variable for debugging>*/
  tachCount2();// do the tach maths for one side
  tachCount3();// and the other side
  //</Do this every time the code runs>
  //<Only if we are stopping>
  if (trk - loc <= decel * spd) { //if we are running out of track
    Serial.print("Break");
    A.brake(); //brake the motors
    B.brake(); //brake the motors
    digitalWrite(brkpin, HIGH); //fire the solenoids
    //</Only if we are stopping>
    //<if there is track left>
  } else {
    //<set the motor Speeds>
    if (spd2 > spd3) {//if motor A is going faster
      Aspd--;//slow it down
    } else if (spd2 < spd3) { //guess
      Bspd--;//Just guess
    }//I hope you cant miss
    //here it gets interesting
    //<make sure the motors are a full speed>
    if (Aspd <= Bspd && Bspd < 255) { //if Bspeed is the highest one, and there is room to raise it
      Aspd += 255 - Bspd; //increase Aspd by the difference between Bspd and the maximum possible value
      Bspd += 255 - Bspd; //Look up, and guess
    } else if (Bspd <= Aspd && Aspd < 255) { ///if Aspeed is the highest one, and there is room to raise it
      Aspd += 255 - Aspd; //increase ABspd by the difference between Bspd and the maximum possible value
      Bspd += 255 - Aspd; //There is one difference, do you see it?
    }
    //</make sure the motors are a full speed>
    //</Set the motor Speeds>
    A.drive(Aspd); //turn the motor on
    B.drive(Bspd); //turn the motor on
  }
  //</if there is track left>
}

void tachLog2() {//interrupt for tachometer
  lastTach2 = thisTach2;//move thisTach into lastTach to make room for the new reading
  thisTach2 = millis();// get the new reading
}

void tachLog3() { //interrupt for tachometer
  lastTach3 = thisTach3;//move thisTach into lastTach to make room for the new reading
  thisTach3 = millis();// get the new reading
}

void tachCount2() {
  //<local variables>
  //I use local variables so that if tachcount is called in the middle of the function it wont make the maths go poof.
  lclLast = lastTach2;
  lclThis = thisTach2;
  //</local Variables>
  spd2 = wedge / (lclThis - lclLast) * 100; //cm/ms, //wedge = dist, //* 100 is to avoid floating point stuff
  if (!(lclThis == thisTach2)) {//if tachLog1 has been called since we started
    tachCount2();//do this function again
    //I love recursion
  }
}

void tachCount3() {
  //<local variables>
  //I use local variables so that if tachcount is called in the middle of the function it wont make the maths go poof.
  lclLast = lastTach3;
  lclThis = thisTach3;
  //</local Variables>
  spd3 = wedge / (lclThis - lclLast) * 100; //cm/ms, //wedge = dist, //* 100 is to avoid floating point stuff
  if (!(lclThis == thisTach2)) {//if tachLog1 has been called since we started
    tachCount3();//do this function again
    //I love recursion
  }
}
