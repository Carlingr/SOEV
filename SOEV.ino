#define brkpin 10
#define tachpin 2

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
