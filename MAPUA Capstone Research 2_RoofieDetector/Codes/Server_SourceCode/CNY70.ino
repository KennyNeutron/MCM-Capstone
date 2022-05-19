//UPDTAE CNY70 Values
void  cny70_UpdateAllValues() {
  if (value_CNY1() < 100) {
    flag_cny1 = true;
  } else {
    flag_cny1 = false;
  }

  if (value_CNY2() < 100) {
    flag_cny2 = true;
  } else {
    flag_cny2 = false;
  }

  if (value_CNY3() < 100) {
    flag_cny3 = true;
  } else {
    flag_cny3 = false;
  }

  if (value_CNY4() < 100) {
    flag_cny4 = true;
  } else {
    flag_cny4 = false;
  }

  if (value_CNY5() < 100) {
    flag_cny5 = true;
  } else {
    flag_cny5 = false;
  }

  if (value_CNY6() < 100) {
    flag_cny6 = true;
  } else {
    flag_cny6 = false;
  }
}

//Get CNY70 Values
byte value_CNY1() {
  return  analogRead(cny_mag1);
}

byte  value_CNY2() {
  return  analogRead(cny_mag2);
}

byte  value_CNY3() {
  return  analogRead(cny_mag3);
}

byte  value_CNY4() {
  return  analogRead(cny_mag4);
}

byte  value_CNY5() {
  return  analogRead(cny_mag5);
}

byte  value_CNY6() {
  return  analogRead(cny_mag6);
}
