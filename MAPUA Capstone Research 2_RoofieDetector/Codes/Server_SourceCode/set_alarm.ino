void setup_alarm() {
  delay(100);
setup_alarm_begin:

  lcd.setCursor(0, 0);
  lcd.print("Set Mag"); lcd.print(setup_count); lcd.print(" Alarm");

  lcd.setCursor(1, 6);
  lcd.print(alarm_current_hour);
  lcd.print(":");
  lcd.print(alarm_current_min);

  lcd.setCursor(3, 0);
  if (alarm_current_enable) {
    lcd.print("ENABLE");
  } else {
    lcd.print("DISABLE");
  }


  //pressed UP btn
  if (!status_PBup()) {
    delay(100);
    while (!status_PBup()) { //debounce
      delay(100);
    }

    switch (setup_nextCount) {
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
        alarm_current_enable = !alarm_current_enable; //flip the enable/disable status of the current alarm
        break;
    }

  }


  //pressed DOWN btn
  if (!status_PBdown()) {
    delay(100);
    while (!status_PBdown()) { //debounce
      delay(100);
    }

    switch (setup_nextCount) {
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
        alarm_current_enable = !alarm_current_enable; //flip the enable/disable status of the current alarm
        break;
    }

  }


  //variable overflow control
  if (alarm_current_hour > 23 && alarm_current_hour < 100) {
    alarm_current_hour = 0;
  } else if (alarm_current_hour == 255) {
    alarm_current_hour = 23;
  }

  if (alarm_current_min > 59 && alarm_current_min < 100) {
    alarm_current_min = 0;
  } else if (alarm_current_min == 255) {
    alarm_current_min = 59;
  }

  if (alarm_current_pillNum > 3) {
    alarm_current_pillNum = 1;
  } else if (alarm_current_pillNum <= 0) {
    alarm_current_pillNum = 3;
  }

  //pressed NEXT btn
  if (!status_PBnext()) {
    delay(100);
    while (!status_PBnext()) { //debounce
      delay(100);
    }
    setup_nextCount++;

    //overflow control
    if (setup_nextCount >= 5) {
      setup_nextCount = 1;
    }
  }

  //pressed SET btn
  if (!status_PBset()) {
    delay(100);
    while (!status_PBset()) { //debounce
      delay(100);
    }
    set_alarm(setup_count, alarm_current_hour, alarm_current_min, alarm_current_pillNum, alarm_current_enable);
    setup_nextCount = 0;
    setup_count++;

    switch (setup_count) {
      case 2:
        setup_nextCount = 0;
        alarm_current_hour = alarm_mag2_hour;
        alarm_current_min = alarm_mag2_min;
        alarm_current_pillNum = alarm_mag2_pillNum;
        alarm_current_enable = alarm_mag2_enable;
        break;
      case 3:
        setup_nextCount = 0;
        alarm_current_hour = alarm_mag3_hour;
        alarm_current_min = alarm_mag3_min;
        alarm_current_pillNum = alarm_mag3_pillNum;
        alarm_current_enable = alarm_mag3_enable;
        break;
      case 4:
        setup_nextCount = 0;
        alarm_current_hour = alarm_mag4_hour;
        alarm_current_min = alarm_mag4_min;
        alarm_current_pillNum = alarm_mag4_pillNum;
        alarm_current_enable = alarm_mag4_enable;
        break;
      case 5:
        setup_nextCount = 0;
        alarm_current_hour = alarm_mag5_hour;
        alarm_current_min = alarm_mag5_min;
        alarm_current_pillNum = alarm_mag5_pillNum;
        alarm_current_enable = alarm_mag5_enable;
        break;
      case 6:
        setup_nextCount = 0;
        alarm_current_hour = alarm_mag6_hour;
        alarm_current_min = alarm_mag6_min;
        alarm_current_pillNum = alarm_mag6_pillNum;
        alarm_current_enable = alarm_mag6_enable;
        break;
    }
  }

  if (setup_count >= 7) {
    goto setup_alarm_exit;
  } else {
    goto setup_alarm_begin;
  }

setup_alarm_exit:
  delay(100);
}

void set_alarm(uint8_t setup_cnt, uint8_t HH, uint8_t MM, uint8_t pillNum, bool enable) {
  switch (setup_cnt) {
    case 1:
      alarm_mag1_hour = HH;
      alarm_mag1_min = MM;
      alarm_mag1_pillNum = pillNum;
      alarm_mag1_enable = enable;
      break;
    case 2:
      alarm_mag2_hour = HH;
      alarm_mag2_min = MM;
      alarm_mag2_pillNum = pillNum;
      alarm_mag2_enable = enable;
      break;
    case 3:
      alarm_mag3_hour = HH;
      alarm_mag3_min = MM;
      alarm_mag3_pillNum = pillNum;
      alarm_mag3_enable = enable;
      break;
    case 4:
      alarm_mag4_hour = HH;
      alarm_mag4_min = MM;
      alarm_mag4_pillNum = pillNum;
      alarm_mag4_enable = enable;
      break;
    case 5:
      alarm_mag5_hour = HH;
      alarm_mag5_min = MM;
      alarm_mag5_pillNum = pillNum;
      alarm_mag5_enable = enable;
      break;
    case 6:
      alarm_mag6_hour = HH;
      alarm_mag6_min = MM;
      alarm_mag6_pillNum = pillNum;
      alarm_mag6_enable = enable;
      break;
  }
}
