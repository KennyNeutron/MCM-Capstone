//ROOFIE NOTIFIER CAPSTONE PROJECT- CLIENT
//04-11-2022
#include "Arduino.h"
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define push_button 2
#define vibration 3


RF24 radio(9, 10); // CE-CSN

const byte RFaddress[6] = "RNCP1"; //RADIO ADDRESS abbv. ---> Roofie Notifier Capstone Project #1
const int CUPaddress = 0x0001 ; //CUP Unique Address composed of 2 bytes (0x0000 to 0xFFFF) and must be

bool PB_state = true;

void setup() {
  pinMode(push_button, INPUT);
  pinMode(vibration, OUTPUT);

  analogWrite(vibration, 0);

  radio.begin();
  radio.openWritingPipe(RFaddress);
  radio.setPALevel(RF24_PA_MAX);
  radio.stopListening();

}

void loop() {
  String str_CUPaddress;

  str_CUPaddress = String(CUPaddress);

  char char_CUPaddress[str_CUPaddress.length()];

  sprintf(char_CUPaddress, "%00d", CUPaddress);

  char msg[] = " ";

  char radio_msg[sizeof(char_CUPaddress) + sizeof(msg)];

  sprintf(radio_msg, "%s", msg, char_CUPaddress);

  PB_state = digitalRead(push_button);

  if (!PB_state) {
    delay(100);
    for (int a = 0; a <= 5; a++) {
      radio.write(&radio_msg, sizeof(radio_msg));
      delay(100);
    }

  }

  analogWrite(vibration, 120); //Haptic Feedback using vibration motor
  delay(1500);
  analogWrite(vibration, 0);
  delay(1000);

  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();

  // The micro controller enters sleep mode if the user continue to press the button to save power
  sleep_mode();


  //The whole system turns off and resets if the button is let go
  delay(30000);


}
