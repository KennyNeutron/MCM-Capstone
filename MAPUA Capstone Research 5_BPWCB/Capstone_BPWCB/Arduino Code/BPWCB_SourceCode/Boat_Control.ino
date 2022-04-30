//Boat Control
void boat_AutoRun() {
  int counter = 0;
  int count_limit = 240;



  while (count_limit > 0) {
    counter = 0;
    while (counter < count_limit && sr04_getDistance() > 100) {
      boat_runForward();
      counter++;
      delay(500);
      if (get_IRStatus()) {
        //THE BIN IS FULL
        goto FullBin_stopAUTORUN;
      }
      if(get_BatteryVoltage()<=3.5){
        goto BatteryLow_stopAUTORUN;
      }
    }

    boat_stop();
    delay(3000);
    boat_faceEAST();

    counter = 0;
    while (counter < count_limit && sr04_getDistance() > 100) {
      boat_runForward();
      counter++;
      delay(500);
      if (get_IRStatus()) {
        //THE BIN IS FULL
        goto FullBin_stopAUTORUN;
      }
      if(get_BatteryVoltage()<=3.5){
        goto BatteryLow_stopAUTORUN;
      }
    }


    boat_stop();
    delay(3000);
    boat_faceSOUTH();

    counter = 0;
    while (counter < count_limit && sr04_getDistance() > 100) {
      boat_runForward();
      counter++;
      delay(500);
      if (get_IRStatus()) {
        //THE BIN IS FULL
        goto FullBin_stopAUTORUN;
      }
      if(get_BatteryVoltage()<=3.5){
        goto BatteryLow_stopAUTORUN;
      }
    }


    boat_stop();
    delay(3000);
    boat_faceWEST();

    counter = 0;
    while (counter < count_limit && sr04_getDistance() > 100) {
      boat_runForward();
      counter++;
      delay(500);
      if (get_IRStatus()) {
        //THE BIN IS FULL
        goto FullBin_stopAUTORUN;
      }
      if(get_BatteryVoltage()<=3.5){
        goto BatteryLow_stopAUTORUN;
      }
    }


    boat_stop();
    delay(3000);
    boat_faceNORTH();

    counter = 0;
    while (counter < count_limit && sr04_getDistance() > 100) {
      boat_runForward();
      counter++;
      delay(500);
      if (get_IRStatus()) {
        //THE BIN IS FULL
        goto FullBin_stopAUTORUN;
      }
      if(get_BatteryVoltage()<=3.5){
        goto BatteryLow_stopAUTORUN;
      }
    }

    count_limit -= 10; //decrease going forward for 5seconds
  }

  //NOW AT THE CENTER
  delay(3000);

  TextMessage = "I'm done cleaning! /n I will now return to home positon /n -BPWCB"; //text for the message.
  Sim800l.sendSms(PhoneNumber, TextMessage);
  //RETURN HOME
  stop_conveyor();
  boat_returnHOME();

  goto run_EXIT;


  //INTERRUPT OPERATION THE BIN IS FULL
FullBin_stopAUTORUN:
  delay(3000);
  TextMessage = "My bin is full! /n I will now return to home positon /n -BPWCB"; //text for the message.
  Sim800l.sendSms(PhoneNumber, TextMessage);
  //RETURN HOME
  stop_conveyor();
  boat_returnHOME();
  goto run_EXIT;

BatteryLow_stopAUTORUN:
  delay(3000);
  TextMessage = "My Battery is Getting LOW! /n I will now return to home positon /n -BPWCB"; //text for the message.
  Sim800l.sendSms(PhoneNumber, TextMessage);
  //RETURN HOME
  stop_conveyor();
  boat_returnHOME();

run_EXIT:
  delay(1000);

}

//the boat will go HOME
void boat_returnHOME() {
  getLocation();

  if (gps_CURRENTLat >= gps_HOMELat) {
    boat_faceWEST();
  } else if (gps_CURRENTLat <= gps_HOMELat) {
    boat_faceEAST();
  }

  while (gps_CURRENTLat >= (gps_HOMELat + 5.00) || gps_CURRENTLat <= (gps_HOMELat - 5.00)) {
    getLocation();
    boat_runForward();
    delay(3000);
    boat_stop();
  }

  boat_stop();
  delay(3000);
  boat_faceSOUTH();

  while (gps_CURRENTLong >= (gps_HOMELong + 5.00) || gps_CURRENTLong <= (gps_HOMELong - 5.00)) {
    getLocation();
    boat_runForward();
    delay(3000);
    boat_stop();
  }


  TextMessage = "I'm back at home position! /n -BPWCB"; //text for the message.
  Sim800l.sendSms(PhoneNumber, TextMessage);

}

//the boat will face NORTH
void boat_faceNORTH() {
  delay(100);
bfn_start:
  get_gyrodata();

  //FACING NORTH?
  if (gyroX >= 85 && gyroY <= 95) {
    goto bfn_exit;
  } else {
    rudder_left();
    ActivatePropeller(80);
    delay(3000);
    goto bfn_start;
  }
bfn_exit:
  delay(100);
}

//the boat will face SOUTH
void boat_faceSOUTH() {
  delay(100);
bfs_start:
  get_gyrodata();

  //FACING SOUTH?
  if (gyroX >= 85 && gyroY <= 95) {
    goto bfs_exit;
  } else {
    rudder_left();
    ActivatePropeller(80);
    delay(3000);
    goto bfs_start;
  }
bfs_exit:
  delay(100);
}


//the boat will face WEST
void boat_faceWEST() {
  delay(100);
bfw_start:
  get_gyrodata();

  //FACING WEST?
  if (gyroX >= 85 && gyroY <= 95) {
    goto bfw_exit;
  } else {
    rudder_left();
    ActivatePropeller(80);
    delay(3000);
    goto bfw_start;
  }
bfw_exit:
  delay(100);
}

//the boat will face EAST
void boat_faceEAST() {
  delay(100);
bfe_start:
  get_gyrodata();

  //FACING RIGHT?
  if (gyroX >= 140 && gyroY <= 130) {
    goto bfe_exit;
  } else {
    rudder_left();
    ActivatePropeller(80);
    delay(3000);
    goto bfe_start;
  }

bfe_exit:
  delay(100);
}

//the boat will run FORWARD
void boat_runForward() {
  rudder_forward();
  ActivatePropeller(100);
}

//the boat will STOP
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
