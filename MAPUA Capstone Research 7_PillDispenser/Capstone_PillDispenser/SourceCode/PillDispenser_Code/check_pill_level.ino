//check for LOW # of pill
void CheckPillLevel() {
  cny70_UpdateAllValues();
  if (flag_cny1) {
    lcd.setCursor(0, 3);
    lcd.print("LOW:");
    lcd.print("1");
  }
  if (flag_cny2) {
    lcd.setCursor(0, 3);
    lcd.print("LOW:");
    lcd.setCursor(5, 3);
    lcd.print("2");
  }
  if (flag_cny3) {
    lcd.setCursor(0, 3);
    lcd.print("LOW:");
    lcd.setCursor(6, 3);
    lcd.print("3");
  }
  if (flag_cny4) {
    lcd.setCursor(0, 3);
    lcd.print("LOW:");
    lcd.setCursor(7, 3);
    lcd.print("4");
  }
  if (flag_cny5) {
    lcd.setCursor(0, 3);
    lcd.print("LOW:");
    lcd.setCursor(8, 3);
    lcd.print("5");
  }
  if (flag_cny6) {
    lcd.setCursor(0, 3);
    lcd.print("LOW:");
    lcd.setCursor(9, 3);
    lcd.print("6");
  }
}
