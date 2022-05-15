
int get_traverseSpeed() {
  return analogRead(traverse_knob);
}

byte get_traverseSpeedPercent() {
  int traverse_PotVal = analogRead(traverse_knob);
  return map(traverse_PotVal, 0, 1023, 0 , 100);
}

int get_augerSpeed() {
  return analogRead(auger_knob);
}

byte get_augerSpeedPercent() {
  int auger_PotVal = analogRead(auger_knob);
  return map(auger_PotVal, 0, 1023, 0, 100);
}


void run_Spinner(){
  digitalWrite(md_in1, 1);  //Activate the Spinner
  delay(3000);
  digitalWrite(md_in1, 0);  //Turn Off after 3 seconds
}

void run_Traverse(int traverseSpeed){
   analogWrite(md_in3, traverseSpeed);
}

void stop_Traverse(){
  analogWrite(md_in3, 0);
}

void run_Auger(int augerSpeed){
  int customDelay=map(augerSpeed, 0, 1023, 300, 4000);
  
  digitalWrite(auger_stepPin, HIGH);
  delay(customDelay);
  digitalWrite(auger_stepPin, LOW);
  delay(customDelay);
}
