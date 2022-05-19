//check for alarms
bool CheckForAlarm() {
  //alarm for mag1
  if (alarm_mag1_enable && rtc.getHour(h12Flag, pmFlag) == alarm_mag1_hour && rtc.getMinute() == alarm_mag1_min) {
    alarm_hasStarted = true;
    dispense(1, alarm_mag1_pillNum); //dispense the set # of pills
  }

  //alarm for mag2
  if (alarm_mag2_enable && rtc.getHour(h12Flag, pmFlag) == alarm_mag2_hour && rtc.getMinute() == alarm_mag2_min) {
    alarm_hasStarted = true;
    dispense(2, alarm_mag2_pillNum); //dispense the set # of pills
  }

  //alarm for mag3
  if (alarm_mag3_enable && rtc.getHour(h12Flag, pmFlag) == alarm_mag3_hour && rtc.getMinute() == alarm_mag3_min) {
    alarm_hasStarted = true;
    dispense(3, alarm_mag3_pillNum); //dispense the set # of pills
  }

  //alarm for mag4
  if (alarm_mag4_enable && rtc.getHour(h12Flag, pmFlag) == alarm_mag4_hour && rtc.getMinute() == alarm_mag4_min) {
    alarm_hasStarted = true;
    dispense(4, alarm_mag4_pillNum); //dispense the set # of pills
  }

  //alarm for mag5
  if (alarm_mag5_enable && rtc.getHour(h12Flag, pmFlag) == alarm_mag5_hour && rtc.getMinute() == alarm_mag5_min) {
    alarm_hasStarted = true;
    dispense(5, alarm_mag5_pillNum); //dispense the set # of pills
  }

  //alarm for mag6
  if (alarm_mag6_enable && rtc.getHour(h12Flag, pmFlag) == alarm_mag6_hour && rtc.getMinute() == alarm_mag6_min) {
    alarm_hasStarted = true;
    dispense(6, alarm_mag6_pillNum); //dispense the set # of pills
  }

  if (alarm_hasStarted) {
    return true;
  } else {
    return false;
  }
}

//Notify LED only
void alarmLED() {
  digitalWrite(redLED, HIGH);
  delay(500);
  digitalWrite(redLED, LOW);
  delay(500);
}

//Notify both
void alarmNow() {
  digitalWrite(redLED, HIGH);
  tone(buzzer, 1000);
  delay(1000);
  digitalWrite(redLED, LOW);
  noTone(buzzer);
  delay(1000);
}
