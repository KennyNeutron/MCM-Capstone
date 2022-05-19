void dispense(uint8_t magSlot, uint8_t pills) {
  while (pills > 0) {
    switch (magSlot) {
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
void dispense_slot1() {
  digitalWrite(Mag1_Sol1, 0); //Lock the Upper Solenoid
  delay(2000);
  digitalWrite(Mag1_Sol2, 1); //Release the Pill (by unlocking the Lower Solenoid)
  delay(2000);
  digitalWrite(Mag1_Sol2, 0); //Lock the Lower Solenoid
  delay(2000);
  digitalWrite(Mag1_Sol1, 1); //Release the Upper Solenoid
}

void dispense_slot2() {
  digitalWrite(Mag2_Sol1, 0); //Lock the Upper Solenoid
  delay(2000);
  digitalWrite(Mag2_Sol2, 1); //Release the Pill (by unlocking the Lower Solenoid)
  delay(2000);
  digitalWrite(Mag2_Sol2, 0); //Lock the Lower Solenoid
  delay(2000);
  digitalWrite(Mag2_Sol1, 1); //Release the Upper Solenoid
}

void dispense_slot3() {
  digitalWrite(Mag3_Sol1, 0); //Lock the Upper Solenoid
  delay(2000);
  digitalWrite(Mag3_Sol2, 1); //Release the Pill (by unlocking the Lower Solenoid)
  delay(2000);
  digitalWrite(Mag3_Sol2, 0); //Lock the Lower Solenoid
  delay(2000);
  digitalWrite(Mag3_Sol1, 1); //Release the Upper Solenoid
}

void dispense_slot4() {
  digitalWrite(Mag4_Sol1, 0); //Lock the Upper Solenoid
  delay(2000);
  digitalWrite(Mag4_Sol2, 1); //Release the Pill (by unlocking the Lower Solenoid)
  delay(2000);
  digitalWrite(Mag4_Sol2, 0); //Lock the Lower Solenoid
  delay(2000);
  digitalWrite(Mag4_Sol1, 1); //Release the Upper Solenoid
}

void dispense_slot5() {
  digitalWrite(Mag5_Sol1, 0); //Lock the Upper Solenoid
  delay(2000);
  digitalWrite(Mag5_Sol2, 1); //Release the Pill (by unlocking the Lower Solenoid)
  delay(2000);
  digitalWrite(Mag5_Sol2, 0); //Lock the Lower Solenoid
  delay(2000);
  digitalWrite(Mag5_Sol1, 1); //Release the Upper Solenoid
}

void dispense_slot6() {
  digitalWrite(Mag6_Sol1, 0); //Lock the Upper Solenoid
  delay(2000);
  digitalWrite(Mag6_Sol2, 1); //Release the Pill (by unlocking the Lower Solenoid)
  delay(2000);
  digitalWrite(Mag6_Sol2, 0); //Lock the Lower Solenoid
  delay(2000);
  digitalWrite(Mag6_Sol1, 1); //Release the Upper Solenoid
}
