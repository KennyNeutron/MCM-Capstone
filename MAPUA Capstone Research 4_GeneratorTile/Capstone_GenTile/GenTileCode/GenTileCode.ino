//Generator Tile on ATTiny45
//04-20-2022

#include <avr/sleep.h>
#include <avr/wdt.h>

#define LED_Green 0
#define LED_Red   1
#define WakePin   3

#define PiezoIn   A2


int PiezoVal = 0;
double PiezoVoltage = 0;
bool ActiveFlag = true;



void setup() {
  pinMode(LED_Green, OUTPUT);
  pinMode(LED_Red, OUTPUT);
  pinMode(PiezoIn, INPUT);

  digitalWrite(LED_Green, LOW); //GRN LED OFF
  digitalWrite(LED_Red, LOW); //RED LED OFF

  PiezoVal = analogRead(PiezoIn);

  PiezoVoltage = map(PiezoVal, 0, 1023, 0, 5.0);


  wdt_enable(WDTO_8S);
}

void loop() {
  wdt_reset();
  
  if (PiezoVoltage > 3.3) {
    ActiveFlag = true;
  } else {
    ActiveFlag = false;
  }

  if (ActiveFlag) {
    digitalWrite(LED_Green, HIGH);
    delay(600);
    digitalWrite(LED_Green, LOW);
  } else {
    digitalWrite(LED_Red, HIGH);
    delay(600);
    digitalWrite(LED_Red, LOW);
  }
  sleepNow();

}


void wakeUpNow() {
  // execute code here after wake-up before returning to the loop()
  PiezoVal = analogRead(PiezoIn);

  PiezoVoltage = map(PiezoVal, 0, 1023, 0, 5.0);
}

void sleepNow() {
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);   // sleep mode is set here
  sleep_enable();          // enables the sleep bit in the mcucr register
  attachInterrupt(3, wakeUpNow, LOW); // use interrupt 3 (pin 3) and run function
  sleep_mode();            // here the device is actually put to sleep!!
  // THE PROGRAM CONTINUES FROM HERE AFTER WAKING UP
  sleep_disable();         // first thing after waking from sleep: disable sleep...
  detachInterrupt(3);      // disables interrupt 3 on pin 3 so the wakeUpNow code will not be executed during normal running time.
}
