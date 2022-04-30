//Boat Control
void boat_runLeft() {
  rudder_left();
  ActivatePropeller(80);
}

void boat_runRight() {
  rudder_right();
  ActivatePropeller(100);
}

void boat_runForward() {
  rudder_forward();
  ActivatePropeller(100);
}

void boat_stop() {
  DeactivatePropeller();
}


//Note boat_speed must be between 0-100
void ActivatePropeller(byte boat_speed) {
  analogWrite(HB1_enA, map(boat_speed, 0, 100, 0 , 255));  //Propeller1
  analogWrite(HB1_enB, map(boat_speed, 0, 100, 0 , 255));  //Propeller2
  delay(100);

  digitalWrite(HB1_in1, LOW);   //Propeller1
  digitalWrite(HB1_in2, HIGH);  //Propeller1
  digitalWrite(HB1_in3, LOW);   //Propeller2
  digitalWrite(HB1_in4, HIGH);  //Propeller2
}

void DeactivatePropeller() {
  analogWrite(HB1_enA, 0);  //Propeller1
  analogWrite(HB1_enB, 0);  //Propeller2
  delay(100);

  digitalWrite(HB1_in1, LOW);   //Propeller1
  digitalWrite(HB1_in2, LOW);  //Propeller1
  digitalWrite(HB1_in3, LOW);   //Propeller2
  digitalWrite(HB1_in4, LOW);  //Propeller2
}




//Initialize Servo Position
void rudder_init() {
  for (rudder_pos = 0; rudder_pos <= 180; rudder_pos++) {
    servoRudder.write(rudder_pos);
    delay(15);
  }

  for (rudder_pos = 180; rudder_pos >= 90; rudder_pos--) {
    servoRudder.write(rudder_pos);
    delay(15);
  }
}


//Rudder Control (For Direction)

//Rudder Angle must be 90degrees to go FORWARD
void rudder_forward() {
  if (servoRudder.read() != 90) {
    if (servoRudder.read() > 90) {
      for (rudder_pos = servoRudder.read(); rudder_pos >= 90; rudder_pos--) {
        servoRudder.write(rudder_pos);
        delay(15);
      }
    } else if (servoRudder.read() < 90) {
      for (rudder_pos = servoRudder.read(); rudder_pos <= 90; rudder_pos++) {
        servoRudder.write(rudder_pos);
        delay(15);
      }
    }
  }
}


//Rudder Angle must be 45degrees to go LEFT
void rudder_left() {
  for (rudder_pos = servoRudder.read(); rudder_pos >= 45; rudder_pos--) {
    servoRudder.write(rudder_pos);
    delay(15);
  }
}

//Rudder Angle must be 135degrees to go RIGHT
void rudder_right() {
  for (rudder_pos = servoRudder.read(); rudder_pos <= 135; rudder_pos++) {
    servoRudder.write(rudder_pos);
    delay(15);
  }
}



//Conveyor Control
//NOTE: Conveyor speed must be 0-100
void start_conveyor(byte conveyor_speed) {
  analogWrite(HB2_enA, map(conveyor_speed, 0, 100, 0, 255));
  digitalWrite(HB2_in1, LOW);
  digitalWrite(HB2_in2, HIGH);
}

void stop_conveyor() {
  analogWrite(HB2_enA, 0);
  digitalWrite(HB2_in1, LOW);
  digitalWrite(HB2_in2, LOW);
}
