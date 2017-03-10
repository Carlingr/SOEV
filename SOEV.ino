unsigned long trk = 1000; //use cm
#define threshold 120

//<define the IO pins for easier Refrencce>
//2=A,3=B
// #define is good for IO pins because #define and IO pins are both hard wiring
#define brkpin 10
#define tachpin 3
//</define the IO pins for easier Refrencce>
#define wedge 10.35 //distance around wheel between inturrupts, ~4.14cm, 

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

// Initializing motors.  The library will allow you to initialize as many
// motors as you have memory for.  If you are using functions like forward
// that take 2 motors as arguements you can either write new functions or
// call the function more than once.
Motor A = Motor(AIN1, AIN2, PWMA, -1, STBY);//this runs forwards
Motor B = Motor(BIN1, BIN2, PWMB, 1, STBY);//this runs backwards
//</Init Motor Drivers>

//<Init Variables>
//Floats are evil due to slow computation speeds. AVOID!
volatile float loc; //use cm
byte spd; //Speed used by motors
#define PHOTO_PIN_1 A0
#define LEDS_PIN 2
//</Init Variables>


void setup() {
  pinMode(LEDS_PIN, OUTPUT);
  // Turn on output LEDs
  digitalWrite(LEDS_PIN, HIGH);
  Serial.begin(250000);//Start Serial
  //<turn on LED 13 to signify init>
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  //</turn on LED 13 to signify init>
  //<pin Modes>
  pinMode(brkpin, OUTPUT);
  pinMode(tachpin, INPUT);
  //</pin Modes>
  A.drive(spd); //turn the motor on
  B.drive(spd); //turn the motor on
  digitalWrite(13, LOW);
}

void loop() {
  if (analogRead(PHOTO_PIN_1) > 120) {
    loc += wedge;
    while (analogRead(PHOTO_PIN_1) > 120) {
      Serial.print(analogRead(PHOTO_PIN_1));
      Serial.println("holding");
    }
  }
  //<Do this every time the code runs>
  //*<log all the variables for debuging>
  Serial.print(millis());
  Serial.print(",");
  Serial.print(loc);
  Serial.print(",");
  Serial.print(analogRead(PHOTO_PIN_1));
  Serial.print(",");
  Serial.println(spd);
  //</log all the variable for debugging>*/
  //</Do this every time the code runs>
  //<Only if we are stopping>
  if (loc >= trk) { //if we are running out of track //SHOULD BE trk - loc <= decel * spd
    Serial.print("Break");
    A.brake(); //brake the motors
    B.brake(); //brake the motors
    digitalWrite(brkpin, HIGH); //fire the solenoids
    //</Only if we are stopping>
  } else { //<if there is track left>
    A.drive(spd); //turn the motor on
    B.drive(spd); //turn the motor on
  }
  //</if there is track left>
}
