//ROOFIE NOTIFIER CAPSTONE PROJECT- SERVER
//04-11-2022
#include <Wire.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#include "U8glib.h"

#define push_button 2
#define redLED 2
#define buzzer 3

RF24 radio(9, 10); // CE-CSN

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE | U8G_I2C_OPT_DEV_0); //OLED Display I2C / TWI

const byte RFaddress[6] = "RNCP1"; //RADIO ADDRESS abbv. ---> Roofie Notifier Capstone Project #1


String RadioMessage = "";
String prevMessage = "";

String str_user[] = "";

bool PB_state = true;
int num_user = 0;


void setup() {
  pinMode(push_button, INPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(redLED, OUTPUT);

  digitalWrite(redLED, HIGH); //turn OFF LED (active LOW);
  noTone(buzzer);

  //u8g.setRot180();  //Rotation
  if ( u8g.getMode() == U8G_MODE_R3G3B2 ) {
    u8g.setColorIndex(255);     // white
  }
  else if ( u8g.getMode() == U8G_MODE_GRAY2BIT ) {
    u8g.setColorIndex(3);         // max intensity
  }
  else if ( u8g.getMode() == U8G_MODE_BW ) {
    u8g.setColorIndex(1);         // pixel on
  }
  else if ( u8g.getMode() == U8G_MODE_HICOLOR ) {
    u8g.setHiColorByRGB(255, 255, 255);
  }

  radio.begin();
  radio.openReadingPipe(0, RFaddress);
  radio.setPALevel(RF24_PA_MAX);
  radio.startListening();


  cli();//stop interrupts

  //set timer0 interrupt at 2kHz
  TCCR0A = 0;// set entire TCCR2A register to 0
  TCCR0B = 0;// same for TCCR2B
  TCNT0  = 0;//initialize counter value to 0
  // set compare match register for 2khz increments
  OCR0A = 124;// = (16*10^6) / (2000*64) - 1 (must be <256)
  // turn on CTC mode
  TCCR0A |= (1 << WGM01);
  // Set CS01 and CS00 bits for 64 prescaler
  TCCR0B |= (1 << CS01) | (1 << CS00);
  // enable timer compare interrupt
  TIMSK0 |= (1 << OCIE0A);

  sei();//allow interrupts

}

void loop() {
  if (radio.available()) {
    radio.read(&RadioMessage, sizeof(RadioMessage));
  }

  if (RadioMessage != prevMessage) {
    num_user++;
    str_user[num_user] = RadioMessage;
  }

  u8g.firstPage();
  do {
    if (num_user > 0) {
      draw(str_user[num_user]);
      tone(buzzer, 1000);  //Buzz
      digitalWrite(redLED, LOW); //Turn ON lED
      delay(1500);
      noTone(buzzer); //Turn OFF Buzzer
      digitalWrite(redLED, HIGH); //Turn OFF LED
      delay(1000);
    }
  } while (u8g.nextPage());

}


void draw(String user_name) {
  u8g.setFont(u8g_font_unifont);
  u8g.setPrintPos(10, 30); u8g.print("NOTICE! CUP#"); u8g.print(user_name); u8g.print("need HELP!");
}


ISR(TIMER0_COMPA_vect) { //timer0 interrupt 2kHz
  cli();

  PB_state = digitalRead(push_button);

  if (!PB_state) {
    delay(100);
    if (num_user > 0) {
      num_user--;
    }
    while (!PB_state) {
      PB_state = digitalRead(push_button);
    }
  }

  sei();
}
