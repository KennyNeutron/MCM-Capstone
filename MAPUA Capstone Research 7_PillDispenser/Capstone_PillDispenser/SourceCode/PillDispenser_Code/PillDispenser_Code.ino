#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <DS3231.h>

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

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


//CNY70 Status Flag
bool  flag_cny1=true;
bool  flag_cny2=true;
bool  flag_cny3=true;
bool  flag_cny4=true;
bool  flag_cny5=true;
bool  flag_cny6=true;


//alarm variables
uint8_t alarm_mag1_hour=0;
uint8_t alarm_mag1_min=0;
uint8_t alarm_mag1_pillNum=1;
bool    alarm_mag1_enable=false;

uint8_t alarm_mag2_hour=0;
uint8_t alarm_mag2_min=0;
uint8_t alarm_mag2_pillNum=1;
bool    alarm_mag2_enable=false;

uint8_t alarm_mag3_hour=0;
uint8_t alarm_mag3_min=0;
uint8_t alarm_mag3_pillNum=1;
bool    alarm_mag3_enable=false;

uint8_t alarm_mag4_hour=0;
uint8_t alarm_mag4_min=0;
uint8_t alarm_mag4_pillNum=1;
bool    alarm_mag4_enable=false;

uint8_t alarm_mag5_hour=0;
uint8_t alarm_mag5_min=0;
uint8_t alarm_mag5_pillNum=1;
bool    alarm_mag5_enable=false;

uint8_t alarm_mag6_hour=0;
uint8_t alarm_mag6_min=0;
uint8_t alarm_mag6_pillNum=1;
bool    alarm_mag6_enable=false;


//setup variables
bool    alarm_setup=false;  //if false no setup, if true goto 
uint8_t setup_count=0;      //used to move between mag
uint8_t setup_nextCount=0; 

uint8_t alarm_current_hour=0;
uint8_t alarm_current_min=0;
uint8_t alarm_current_pillNum=1;
bool    alarm_current_enable=false;

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
  lcd.setCursor(1,6);
  lcd.print(rtc.getHour(h12Flag, pmFlag)); //24-hr
  lcd.print(":");
  lcd.print(rtc.getMinute());
  lcd.print(":");
  lcd.println(rtc.getSecond());

  if(!status_PBset()){
    int btn_delay=0;
    while(!status_PBset()){
      btn_delay++;
      delay(100);
    }
    if(btn_delay>50&&!status_PBset()){
      alarm_setup=true;
    }
  }

  if(alarm_setup){
    setup_count=1;
    setup_nextCount=0;
    alarm_current_hour=alarm_mag1_hour;
    alarm_current_min=alarm_mag1_min;
    alarm_current_pillNum=alarm_mag1_pillNum;
    alarm_current_enable=alarm_mag1_enable;
    set_alarm();
  }

}


void set_alarm(){
  delay(100);
  set_alarm_begin:

  lcd.setCursor(0,0);
  lcd.print("Set Mag");lcd.print(setup_count);lcd.print(" Alarm");

  lcd.setCursor(1,6);
  lcd.print(alarm_current_hour);
  lcd.print(":");
  lcd.print(alarm_current_min);

  lcd.setCursor(3,0);
  if(alarm_current_enable){
    lcd.print("ENABLE");
  }else{
    lcd.print("DISABLE");
  }


  //pressed UP btn
  if(!status_PBup()){
    delay(100);
    while(!status_PBup()){  //debounce
      delay(100);
    }

    switch(setup_nextCount){
        case 1:
          alarm_current_hour++;   //increment the current alarm hour
          break;
        case 2:
          alarm_current_min++;    //increment the current alarm min
          break;
        case 3:
          alarm_current_pillNum++;  //increment the current number of pill to dispense;
          break;
        case 4:
          alarm_current_enable=!alarm_current_enable; //flip the enable/disable status of the current alarm
          break;
    }

  }


  //pressed DOWN btn
  if(!status_PBdown()){
    delay(100);
    while(!status_PBdown()){  //debounce
      delay(100);
    }

    switch(setup_nextCount){
      case 1:
        alarm_current_hour--; //decrement the current alarm hour
        break;
      case 2:
        alarm_current_min--;  //decrement the current alarm min
        break;
      case 3:
        alarm_current_pillNum--;  //decrement the current number of pill to dispense;
        break;
      case 4:
        alarm_current_enable=!alarm_current_enable; //flip the enable/disable status of the current alarm
        break;
    }

  }


//variable overflow control
  if(alarm_current_hour>23 && alarm_current_hour<100){
    alarm_current_hour=0;
  }else if(alarm_current_hour==255){
    alarm_current_hour=23;
  }

  if(alarm_current_min>59 && alarm_current_min<100){
    alarm_current_min=0;
  }else if(alarm_current_min==255){
    alarm_current_min=59;
  }

  if(alarm_current_pillNum>3){
    alarm_current_pillNum=1;
  }else if(alarm_current_pillNum<=0){
    alarm_current_pillNum=3;
  }

  //pressed NEXT btn
  if(!status_PBnext()){
    delay(100);
    while(!status_PBnext()){  //debounce
      delay(100);
    }
    setup_nextCount++;

    //overflow control
    if(setup_nextCount>=5){
      setup_nextCount=1;
    }
  }

  //pressed SET btn
  if(!status_PBset()){
    delay(100);
    while(!status_PBset()){ //debounce
      delay(100);
    }
    set_alarm(setup_count, alarm_current_hour, alarm_current_min,alarm_current_pillNum, alarm_current_enable);
    setup_nextCount=0;
    setup_count++;

    switch(setup_count){
      case 2:
        setup_nextCount=0;
        alarm_current_hour=alarm_mag2_hour;
        alarm_current_min=alarm_mag2_min;
        alarm_current_pillNum=alarm_mag2_pillNum;
        alarm_current_enable=alarm_mag2_enable;
        break;
      case 3:
        setup_nextCount=0;
        alarm_current_hour=alarm_mag3_hour;
        alarm_current_min=alarm_mag3_min;
        alarm_current_pillNum=alarm_mag3_pillNum;
        alarm_current_enable=alarm_mag3_enable;
        break;
      case 4:
        setup_nextCount=0;
        alarm_current_hour=alarm_mag4_hour;
        alarm_current_min=alarm_mag4_min;
        alarm_current_pillNum=alarm_mag4_pillNum;
        alarm_current_enable=alarm_mag4_enable;
        break;
      case 5:
        setup_nextCount=0;
        alarm_current_hour=alarm_mag5_hour;
        alarm_current_min=alarm_mag5_min;
        alarm_current_pillNum=alarm_mag5_pillNum;
        alarm_current_enable=alarm_mag5_enable;
        break;
      case 6:
        setup_nextCount=0;
        alarm_current_hour=alarm_mag6_hour;
        alarm_current_min=alarm_mag6_min;
        alarm_current_pillNum=alarm_mag6_pillNum;
        alarm_current_enable=alarm_mag6_enable;
        break;
    }
  }

  if(setup_count>=7){
    goto set_alarm_exit;
  }else{
    goto set_alarm_begin;
  }

  set_alarm_exit:
  delay(100);


}

void set_alarm(uint8_t setup_cnt,uint8_t HH,uint8_t MM,uint8_t pillNum,bool enable){
    switch(setup_cnt){
      case 1:
        alarm_mag1_hour=HH;
        alarm_mag1_min=MM;
        alarm_mag1_pillNum=pillNum;
        alarm_mag1_enable=enable;
        break;
      case 2:
        alarm_mag2_hour=HH;
        alarm_mag2_min=MM;
        alarm_mag2_pillNum=pillNum;
        alarm_mag2_enable=enable;
        break;
      case 3:
        alarm_mag3_hour=HH;
        alarm_mag3_min=MM;
        alarm_mag3_pillNum=pillNum;
        alarm_mag3_enable=enable;
        break;
      case 4:
        alarm_mag4_hour=HH;
        alarm_mag4_min=MM;
        alarm_mag4_pillNum=pillNum;
        alarm_mag4_enable=enable;
        break;
      case 5:
        alarm_mag5_hour=HH;
        alarm_mag5_min=MM;
        alarm_mag5_pillNum=pillNum;
        alarm_mag5_enable=enable;
        break;
      case 6:
        alarm_mag6_hour=HH;
        alarm_mag6_min=MM;
        alarm_mag6_pillNum=pillNum;
        alarm_mag6_enable=enable;
        break;
    }
}



//Button Status
bool status_PBset(){
  return digitalRead(pb_set);
}

bool status_PBnext(){
  return digitalRead(pb_next);
}

bool status_PBdown(){
  return  digitalRead(pb_down);
}

bool status_PBup(){
  return  digitalRead(pb_up);
}

//UPDTAE CNY70 Values
void  cny70_UpdateAllValues(){
  if(value_CNY1()<100){
    flag_cny1=true;
  }else{
    flag_cny1=false;
  }

  if(value_CNY2()<100){
    flag_cny2=true;
  }else{
    flag_cny2=false;
  }

  if(value_CNY3()<100){
    flag_cny3=true;
  }else{
    flag_cny3=false;
  }

  if(value_CNY4()<100){
    flag_cny4=true;
  }else{
    flag_cny4=false;
  }

  if(value_CNY5()<100){
    flag_cny5=true;
  }else{
    flag_cny5=false;
  }

  if(value_CNY6()<100){
    flag_cny6=true;
  }else{  
    flag_cny6=false;
  }
}

//Get CNY70 Values
byte value_CNY1(){
  return  analogRead(cny_mag1);
}

byte  value_CNY2(){
  return  analogRead(cny_mag2);
}

byte  value_CNY3(){
  return  analogRead(cny_mag3);
}

byte  value_CNY4(){
  return  analogRead(cny_mag4);
}

byte  value_CNY5(){
  return  analogRead(cny_mag5);
}

byte  value_CNY6(){
  return  analogRead(cny_mag6);
}



void dispense(uint8_t magSlot, uint8_t pills){
  while(pills>0){
      switch(magSlot){
        case 1:
          dispense_slot1();
          break;
        case 2:
          dispense_slot2();
          break;
        case 3:
          dispense_slot3();
          break;
        case 4:
          dispense_slot4();
          break;
        case 5:
          dispense_slot5();
          break;
        case 6:
          dispense_slot6();
          break;
        default:
          goto dispense_exit;

      }
  }

dispense_exit:
  delay(10);
}


//Individual Dispensing per Mag
void dispense_slot1(){
    digitalWrite(Mag1_Sol1,0); //Lock the Upper Solenoid
    delay(2000);
    digitalWrite(Mag1_Sol2,1); //Release the Pill (by unlocking the Lower Solenoid)
    delay(2000);
    digitalWrite(Mag1_Sol2,0); //Lock the Lower Solenoid
    delay(2000);
    digitalWrite(Mag1_Sol1,1); //Release the Upper Solenoid
}

void dispense_slot2(){
    digitalWrite(Mag2_Sol1,0); //Lock the Upper Solenoid
    delay(2000);
    digitalWrite(Mag2_Sol2,1); //Release the Pill (by unlocking the Lower Solenoid)
    delay(2000);
    digitalWrite(Mag2_Sol2,0); //Lock the Lower Solenoid
    delay(2000);
    digitalWrite(Mag2_Sol1,1); //Release the Upper Solenoid
}

void dispense_slot3(){
    digitalWrite(Mag3_Sol1,0); //Lock the Upper Solenoid
    delay(2000);
    digitalWrite(Mag3_Sol2,1); //Release the Pill (by unlocking the Lower Solenoid)
    delay(2000);
    digitalWrite(Mag3_Sol2,0); //Lock the Lower Solenoid
    delay(2000);
    digitalWrite(Mag3_Sol1,1); //Release the Upper Solenoid
}

void dispense_slot4(){
    digitalWrite(Mag4_Sol1,0); //Lock the Upper Solenoid
    delay(2000);
    digitalWrite(Mag4_Sol2,1); //Release the Pill (by unlocking the Lower Solenoid)
    delay(2000);
    digitalWrite(Mag4_Sol2,0); //Lock the Lower Solenoid
    delay(2000);
    digitalWrite(Mag4_Sol1,1); //Release the Upper Solenoid
}

void dispense_slot5(){
    digitalWrite(Mag5_Sol1,0); //Lock the Upper Solenoid
    delay(2000);
    digitalWrite(Mag5_Sol2,1); //Release the Pill (by unlocking the Lower Solenoid)
    delay(2000);
    digitalWrite(Mag5_Sol2,0); //Lock the Lower Solenoid
    delay(2000);
    digitalWrite(Mag5_Sol1,1); //Release the Upper Solenoid
}

void dispense_slot6(){
    digitalWrite(Mag6_Sol1,0); //Lock the Upper Solenoid
    delay(2000);
    digitalWrite(Mag6_Sol2,1); //Release the Pill (by unlocking the Lower Solenoid)
    delay(2000);
    digitalWrite(Mag6_Sol2,0); //Lock the Lower Solenoid
    delay(2000);
    digitalWrite(Mag6_Sol1,1); //Release the Upper Solenoid
}
