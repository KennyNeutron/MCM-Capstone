#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DS3231.h>

LiquidCrystal_I2C lcd(0x27, 20, 4); // set the LCD address to 0x27 for a 16 chars and 2 line display

DS3231 rtc;

bool century = false;
bool h12Flag;
bool pmFlag;


//Solenoid Controls
#define Mag1_Sol1 23    //NAMING FORMAT: Magazine 1 Solenoid 1
#define Mag1_Sol2 25

#define Mag2_Sol1 27
#define Mag2_Sol2 29

#define Mag3_Sol1 31
#define Mag3_Sol2 33

#define Mag4_Sol1 35
#define Mag4_Sol2 37

#define Mag5_Sol1 39
#define Mag5_Sol2 41

#define Mag6_Sol1 43
#define Mag6_Sol2 45


//Push Buttons
#define pb_set  5
#define pb_next 4
#define pb_up   3
#define pb_down 2

//CNY70
#define cny_mag1  A0
#define cny_mag2  A1
#define cny_mag3  A2
#define cny_mag4  A3
#define cny_mag5  A4
#define cny_mag6  A5

//Notifiers
#define buzzer  6
#define redLED  7


//CNY70 Status Flag
bool  flag_cny1 = true;
bool  flag_cny2 = true;
bool  flag_cny3 = true;
bool  flag_cny4 = true;
bool  flag_cny5 = true;
bool  flag_cny6 = true;


//alarm variables
uint8_t alarm_mag1_hour = 0;
uint8_t alarm_mag1_min = 0;
uint8_t alarm_mag1_pillNum = 1;
bool    alarm_mag1_enable = false;

uint8_t alarm_mag2_hour = 0;
uint8_t alarm_mag2_min = 0;
uint8_t alarm_mag2_pillNum = 1;
bool    alarm_mag2_enable = false;

uint8_t alarm_mag3_hour = 0;
uint8_t alarm_mag3_min = 0;
uint8_t alarm_mag3_pillNum = 1;
bool    alarm_mag3_enable = false;

uint8_t alarm_mag4_hour = 0;
uint8_t alarm_mag4_min = 0;
uint8_t alarm_mag4_pillNum = 1;
bool    alarm_mag4_enable = false;

uint8_t alarm_mag5_hour = 0;
uint8_t alarm_mag5_min = 0;
uint8_t alarm_mag5_pillNum = 1;
bool    alarm_mag5_enable = false;

uint8_t alarm_mag6_hour = 0;
uint8_t alarm_mag6_min = 0;
uint8_t alarm_mag6_pillNum = 1;
bool    alarm_mag6_enable = false;


//setup variables
bool    alarm_setup = false; //if false no setup, if true goto
uint8_t setup_count = 0;    //used to move between mag
uint8_t setup_nextCount = 0;

uint8_t alarm_current_hour = 0;
uint8_t alarm_current_min = 0;
uint8_t alarm_current_pillNum = 1;
bool    alarm_current_enable = false;


//notify alarm variables
bool    alarm_hasStarted = false;
bool    user_TookThePill = false;
uint8_t hasStarted_count = 0;
uint8_t prev_sec = 0;

void setup() {
  pinMode(Mag1_Sol1, OUTPUT);
  pinMode(Mag1_Sol2, OUTPUT);
  pinMode(Mag2_Sol1, OUTPUT);
  pinMode(Mag2_Sol2, OUTPUT);
  pinMode(Mag3_Sol1, OUTPUT);
  pinMode(Mag3_Sol2, OUTPUT);
  pinMode(Mag4_Sol1, OUTPUT);
  pinMode(Mag4_Sol2, OUTPUT);
  pinMode(Mag5_Sol1, OUTPUT);
  pinMode(Mag5_Sol2, OUTPUT);
  pinMode(Mag6_Sol1, OUTPUT);
  pinMode(Mag6_Sol2, OUTPUT);

  pinMode(pb_set, INPUT_PULLUP);
  pinMode(pb_next, INPUT_PULLUP);
  pinMode(pb_down, INPUT_PULLUP);
  pinMode(pb_up, INPUT_PULLUP);

  pinMode(cny_mag1, INPUT);
  pinMode(cny_mag2, INPUT);
  pinMode(cny_mag3, INPUT);
  pinMode(cny_mag4, INPUT);
  pinMode(cny_mag5, INPUT);
  pinMode(cny_mag6, INPUT);

  pinMode(buzzer, OUTPUT);
  pinMode(redLED, OUTPUT);

  noTone(buzzer);
  digitalWrite(redLED, LOW);


  //Init all solenoids
  digitalWrite(Mag1_Sol1, 1);
  digitalWrite(Mag1_Sol2, 0);
  digitalWrite(Mag2_Sol1, 1);
  digitalWrite(Mag2_Sol2, 0);
  digitalWrite(Mag3_Sol1, 1);
  digitalWrite(Mag3_Sol2, 0);
  digitalWrite(Mag4_Sol1, 1);
  digitalWrite(Mag4_Sol2, 0);
  digitalWrite(Mag5_Sol1, 1);
  digitalWrite(Mag5_Sol2, 0);
  digitalWrite(Mag6_Sol1, 1);
  digitalWrite(Mag6_Sol2, 0);

  Wire.begin();
  lcd.init();

}

void loop() {

  //Displaying Time
  lcd.setCursor(1, 6);
  lcd.print(rtc.getHour(h12Flag, pmFlag)); //24-hr
  lcd.print(":");
  lcd.print(rtc.getMinute());
  lcd.print(":");
  lcd.println(rtc.getSecond());

  if (!status_PBset()) {
    int btn_delay = 0;
    while (!status_PBset()) {
      btn_delay++;
      delay(100);
    }
    if (btn_delay > 50 && !status_PBset()) {
      alarm_setup = true;
    } else if (btn_delay > 3 && btn_delay < 20) {
      user_TookThePill = true;
    }
  }

  if (alarm_setup) {
    setup_count = 1;
    setup_nextCount = 0;
    alarm_current_hour = alarm_mag1_hour;
    alarm_current_min = alarm_mag1_min;
    alarm_current_pillNum = alarm_mag1_pillNum;
    alarm_current_enable = alarm_mag1_enable;
    setup_alarm();
  }


  if (!alarm_hasStarted) {
    if (CheckForAlarm()) {
      prev_sec = rtc.getSecond();
    }
  } else {
    if (rtc.getSecond() != prev_sec) {
      hasStarted_count++;
      prev_sec = rtc.getSecond();
    }
    if (hasStarted_count < 60 && !user_TookThePill) { //if there's an alarm and the pill was not taken yet
      alarmNow();     //Alarm Buzzer + LED for 60s
    } else if (hasStarted_count >= 60 && !user_TookThePill) {
      alarmLED();     //After 60s LED notifier only
    }


    //Disable the alarm after the user took the pill
    if (user_TookThePill) {
      hasStarted_count = false;
      user_TookThePill = false;
    }
  }

  CheckPillLevel();

}
