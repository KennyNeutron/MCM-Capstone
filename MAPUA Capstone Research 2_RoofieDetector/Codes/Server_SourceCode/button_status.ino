//Button Status
bool status_PBset() {
  return digitalRead(pb_set);
}

bool status_PBnext() {
  return digitalRead(pb_next);
}

bool status_PBdown() {
  return  digitalRead(pb_down);
}

bool status_PBup() {
  return  digitalRead(pb_up);
}
