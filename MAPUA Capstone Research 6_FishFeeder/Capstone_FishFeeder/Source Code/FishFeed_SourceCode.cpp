/*
   Fish Feeder Boat
   05-05-2022
*/
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display

//Potentiometers
#define auger_knob A0
#define traverse_knob A1

//Auger Motor (Stepper Motor)
#define auger_stepPin 3
#define auger_dirPin 4

//L298N Motor Driver
#define md_in1 8    //Spinner
#define md_in2 9    //Spinner
#define md_in3 10   //Traverse
#define md_in4 11   //Traverse

//SR04
#define echoPin 5
#define trigPin 6

//IR OS
#define IRpin 2

//LED indicator
#define LEDpin 7

//Buttons
#define buttonDOWN 12
#define buttonUP 13


bool started = false;
bool feeds = true;

int spinner_interval = 5000;

unsigned long prev_millis = 0;


void setup() {
  lcd.init();
  lcd.backlight();

  //Potentiometer
  pinMode(auger_knob, INPUT);
  pinMode(traverse_knob, INPUT);

  //Auger
  pinMode(auger_stepPin, OUTPUT);
  pinMode(auger_dirPin, OUTPUT);
  delay(100);
  digitalWrite(auger_dirPin, HIGH);

  //L298N Motor Driver
  pinMode(md_in1, OUTPUT); //Spinner
  pinMode(md_in2, OUTPUT); //Spinner
  pinMode(md_in3, OUTPUT); //Traverse
  pinMode(md_in4, OUTPUT); //Traverse
  delay(100);
  digitalWrite(md_in1, 0);
  digitalWrite(md_in2, 0);
  analogWrite(md_in3, 0);
  analogWrite(md_in4, 0);

  //SR04
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  //IR
  pinMode(IRpin, INPUT);

  //LED indicator
  pinMode(LEDpin, OUTPUT);
  digitalWrite(LEDpin, 1); //Turn Off LED

  //Buttons
  pinMode(buttonDOWN, INPUT_PULLUP);
  pinMode(buttonUP, INPUT_PULLUP);

  prev_millis = millis();
}

void loop() {

  if (started) {
    run_Traverse(get_traverseSpeed());  //Activate the Traverse Motor based on the speed set

    feeds = IR_status();

    if (!feeds) {  //There's feeds available
      run_Auger(get_augerSpeed());        //Activate the Auger based on the speed set

      if (millis() - prev_millis >= spinner_interval) { //if time passed is equal to the set interval then activate the spinner
        run_Spinner();
      }
      LED_OFF(); //No Notif yet
    } else {
      LED_ON; //Notify
    }

    if (SR04_distance() <= 50) { //If there is an obstacle within 50cm the boat will stop since it reached the end of the line
      started = false; //Stop the Boat
    }

  } else {
    stop_Traverse();
  }


  lcd.setCursor(0, 0);
  lcd.print("AS:"); //Show Auger Speed
  lcd.print(get_augerSpeedPercent());
  lcd.print("%");

  lcd.setCursor(0, 1);
  lcd.print("TS:"); //Show Traverse Speed
  lcd.print(get_traverseSpeedPercent());
  lcd.print("%");

  lcd.setCursor(8, 0);
  lcd.print("SI:"); //Show Spinner Interval
  lcd.print(spinner_interval/1000);

  if (!status_buttonUP() && status_buttonDOWN()) {    //If ONLY button UP is pressed
    delay(100);
    while (!status_buttonUP());
    if (spinner_interval >= 0) {
      spinner_interval = spinner_interval + 5000; ///ADD 5 seconds to the spinner interval
    }
  }

  if (!status_buttonDOWN() && status_buttonUP()) {    //If ONLY button DOWN is pressed
    delay(100);
    while (!status_buttonDOWN());
    if (spinner_interval >= 0) {
      spinner_interval = spinner_interval - 5000; ///SUBTRACT 5 seconds to the spinner interval
    }
  }


  while (!status_buttonUP() && !status_buttonDOWN());

  if (!status_buttonUP() && !status_buttonDOWN()) {    //If BOTH buttons are pressed for 5 seconds then activate the system
    delay(5000);
    if (!status_buttonUP() && !status_buttonDOWN()) {
      started = true;
      delay(300);
    }
  }


}

void LED_ON() {
  digitalWrite(LEDpin, 0); //Turn On LED
}

void LED_OFF() {
  digitalWrite(LEDpin, 1); //Turn Off LED
}

bool status_buttonDOWN() {
  return digitalRead(buttonDOWN);
}

bool status_buttonUP() {
  return digitalRead(buttonUP);
}

//Motors

int get_traverseSpeed() {
  return analogRead(traverse_knob);
}

byte get_traverseSpeedPercent() {
  int traverse_PotVal = analogRead(traverse_knob);
  return map(traverse_PotVal, 0, 1023, 0 , 100);
}

int get_augerSpeed() {
  return analogRead(auger_knob);
}

byte get_augerSpeedPercent() {
  int auger_PotVal = analogRead(auger_knob);
  return map(auger_PotVal, 0, 1023, 0, 100);
}


void run_Spinner(){
  digitalWrite(md_in1, 1);  //Activate the Spinner
  delay(3000);
  digitalWrite(md_in1, 0);  //Turn Off after 3 seconds
}

void run_Traverse(int traverseSpeed){
   analogWrite(md_in3, traverseSpeed);
}

void stop_Traverse(){
  analogWrite(md_in3, 0);
}

void run_Auger(int augerSpeed){
  int customDelay=map(augerSpeed, 0, 1023, 300, 4000);
  
  digitalWrite(auger_stepPin, HIGH);
  delay(customDelay);
  digitalWrite(auger_stepPin, LOW);
  delay(customDelay);
}

//Obstacle

bool IR_status(){
  return digitalRead(IRpin);
}


int SR04_distance() {
  long duration;
  int distance;

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)

  
  return distance;
}