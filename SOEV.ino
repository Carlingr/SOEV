unsigned long tme = 5; //use secs
#define threshold 650

byte Aspd = 255; //Speed used by motorA
byte Bspd = 245; //Speed used by motorB

//<define the IO pins for easier Refrencce>
// #define is good for IO pins because #define and IO pins are both hard wiring
#define brkpin 10

/*<Init Motor Drivers>
  Creadit where cedit is due - This was copied and pasted from the Sparkfun Hookup Guide
  https://github.com/sparkfun/SparkFun_TB6612FNG_Arduino_Library/blob/master/examples/MotorTestRun/MotorTestRun.ino11
*/
#include <SparkFun_TB6612.h> //This is the library for the TB6612 that contains the class Motor and all the functions

/* Initializing motors.  The library will allow you to initialize as many
   motors as you have memory for.  If you are using functions like forward
   that take 2 motors as arguements you can either write new functions or
   call the function more than once.*/
//Order is: IN1, IN2, PWM, OFFSET, STBY
Motor A = Motor(3, 4, 5, 1, 9);//this runs forwards
Motor B = Motor(7, 8, 6, -1, 9);//this runs backwards
//</Init Motor Drivers>
void setup() {
  //<turn on LED 13 to signify init>
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  //</turn on LED 13 to signify init>
  //<pin Modes>
  pinMode(brkpin, OUTPUT);
  //</pin Modes>
  A.drive(Aspd); //turn the motor on
  B.drive(Bspd); //turn the motor on
  digitalWrite(13, LOW);
  delay(tme * 1000);
  A.brake(); //brake the motors
  B.brake(); //brake the motors
  digitalWrite(brkpin, HIGH);
  digitalWrite(13, HIGH);
} void loop() {}
