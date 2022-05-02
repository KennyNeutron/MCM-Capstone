/*
   BPWCP Arduino Code
   04-30-2022
*/
#include <Sim800l.h>
#include <SoftwareSerial.h> //is necesary for the GSM Module library!! 
#include <TinyGPS.h>
#include <Servo.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>



Sim800l Sim800l;  //to declare the GSM Module library
TinyGPS gps;
Servo servoRudder;
Adafruit_MPU6050 mpu;

//Start Button
#define start_button A8

//GREEN LED
#define LED_grn A7

//Battery
#define BatteryPin A1

//SR04 Ultasonic Distance Sensor
#define echoPin 2
#define trigPin 3

//IR Sensor
#define ir_en 22        //IR enable pin
#define ir_status 23    //IR status pin


//HBridge 1 --> Propeller;
#define HB1_enA 6   //Enable A
#define HB1_enB 7   //Enable B
#define HB1_in1 51  //Input 1  
#define HB1_in2 53  //Input 2
#define HB1_in3 52  //Input 3
#define HB1_in4 50  //Input 4
//NOTE: For Easy wiring purposes these are not in chronological order



//HBridge 2 --> Conveyor
#define HB2_enA 5   //Enable A
#define HB2_in1 49  //Input 1
#define HB2_in2 48  //Input 2
//NOTE: EnableB, Input1, and Input2 are not used in this part


//###############Variables#######################

//GSM Variables
char* TextMessage;
char* PhoneNumber;
//bool error; //to catch the response of sendSms

//GPS Variables
float gps_HOMELat;
float gps_HOMELong;
float gps_CURRENTLat;
float gps_CURRENTLong;

//Gyroscope
int gyroX;  //YAW
int gyroY;  //PITCH
int gyroZ;  //ROLL


//Start Button
bool started = false;

//Rudder(Servo) Variables
int rudder_pos = 0;


//###############################################




void setup() {
  //Start Button
  pinMode(start_button, INPUT);

  //LED
  pinMode(LED_grn, OUTPUT);
  delay(100);
  digitalWrite(LED_grn, LOW);

  //Battery Pin
  pinMode(BatteryPin, INPUT);
  

  //SR04 Ultrasonic Sensor
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  //IR Sensor
  pinMode(ir_en, OUTPUT);
  pinMode(ir_status, INPUT);
  delay(100);
  digitalWrite(ir_status, HIGH);

  //HBridge1 --> Proppeler
  pinMode(HB1_enA, OUTPUT);
  pinMode(HB1_enB, OUTPUT);
  pinMode(HB1_in1, OUTPUT);
  pinMode(HB1_in2, OUTPUT);
  pinMode(HB1_in3, OUTPUT);
  pinMode(HB1_in4, OUTPUT);

  //HBridge2 --> Conveyor
  pinMode(HB2_enA, OUTPUT);
  pinMode(HB2_in1, OUTPUT);
  pinMode(HB2_in2, OUTPUT);

  //Rudder
  servoRudder.attach(4);


  //Initialize GSM Module
  Sim800l.begin(); // initializate the library.
  TextMessage = "Testing Sms"; //text for the message.
  PhoneNumber = "09286707466"; //change to a valid number.
  //Sim800l.sendSms("+540111111111","the text go here")


  //Initialize GPS Module
  Serial1.begin(9600);



}



void loop() {
  if (!state_startbutton()) {
    delay(500);
    if (!state_startbutton()) {
      started = !started; //Invert the started status true=start false=stop
    }
    while (!state_startbutton());
  }


  if (started) {
    int countDown = 60;

    //GREEN LED will blink for 30seconds
    while (countDown > 0) {
      digitalWrite(LED_grn, HIGH);
      delay(500);
      digitalWrite(LED_grn, LOW);
      delay(500);
    }
    digitalWrite(LED_grn, HIGH);

    //SAVE THE HOME LOCATION
    getLocation();
    gps_HOMELat = gps_CURRENTLat;
    gps_HOMELong = gps_CURRENTLong;

    String str_HOMELat = String(gps_HOMELat);
    String str_HOMELong = String(gps_HOMELong);

    String str_StartMessage = "BPWCB started at: " + str_HOMELat + "," + str_HOMELong + " /n -BPWCB"; //text for the message.
    int str_len = str_StartMessage.length() + 1;

    char start_message[str_len];


    str_StartMessage.toCharArray(start_message,  str_len);

    Sim800l.sendSms(PhoneNumber, start_message);

    //Start AutoRun to on Boat
    boat_AutoRun();
  }


}

bool state_startbutton() {
  return digitalRead(start_button);
}

bool get_IRStatus() {
  return digitalRead(ir_status);
}

double get_BatteryVoltage(){
  int batt_analog=analogRead(BatteryPin);
  double batt_voltage= map(batt_analog, 0, 1023, 0.00, 7.6);
  return batt_voltage;
}

void getLocation() {
  while (Serial1.available()) {
    if (gps.encode(Serial1.read())) {
      gps.f_get_position(&gps_CURRENTLat, &gps_CURRENTLong);
    }
  }
}

//Initialize Gyroscope
void gyro_init() {
  Serial.begin(115200);
  while (!Serial)
    delay(10); // will pause Zero, Leonardo, etc until serial console opens

  Serial.println("Adafruit MPU6050 test!");

  // Try to initialize!
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  Serial.print("Accelerometer range set to: ");
  switch (mpu.getAccelerometerRange()) {
  case MPU6050_RANGE_2_G:
    Serial.println("+-2G");
    break;
  case MPU6050_RANGE_4_G:
    Serial.println("+-4G");
    break;
  case MPU6050_RANGE_8_G:
    Serial.println("+-8G");
    break;
  case MPU6050_RANGE_16_G:
    Serial.println("+-16G");
    break;
  }
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  Serial.print("Gyro range set to: ");
  switch (mpu.getGyroRange()) {
  case MPU6050_RANGE_250_DEG:
    Serial.println("+- 250 deg/s");
    break;
  case MPU6050_RANGE_500_DEG:
    Serial.println("+- 500 deg/s");
    break;
  case MPU6050_RANGE_1000_DEG:
    Serial.println("+- 1000 deg/s");
    break;
  case MPU6050_RANGE_2000_DEG:
    Serial.println("+- 2000 deg/s");
    break;
  }

  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  Serial.print("Filter bandwidth set to: ");
  switch (mpu.getFilterBandwidth()) {
  case MPU6050_BAND_260_HZ:
    Serial.println("260 Hz");
    break;
  case MPU6050_BAND_184_HZ:
    Serial.println("184 Hz");
    break;
  case MPU6050_BAND_94_HZ:
    Serial.println("94 Hz");
    break;
  case MPU6050_BAND_44_HZ:
    Serial.println("44 Hz");
    break;
  case MPU6050_BAND_21_HZ:
    Serial.println("21 Hz");
    break;
  case MPU6050_BAND_10_HZ:
    Serial.println("10 Hz");
    break;
  case MPU6050_BAND_5_HZ:
    Serial.println("5 Hz");
    break;
  }

  Serial.println("");
  delay(100);
}


//get gyroscope data
void get_gyrodata(){
  /* Get new sensor events with the readings */
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  gyroX= g.gyro.x;
  gyroY= g.gyro.y;
  gyroZ= g.gyro.z;


//UNCOMMENT THIS PART FOR TROUBLESHOOTING PURPOSES ONLY

//  /* Print out the values */
//  Serial.print("Acceleration X: ");
//  Serial.print(a.acceleration.x);
//  Serial.print(", Y: ");
//  Serial.print(a.acceleration.y);
//  Serial.print(", Z: ");
//  Serial.print(a.acceleration.z);
//  Serial.println(" m/s^2");
//
//  Serial.print("Rotation X: ");
//  Serial.print(g.gyro.x);
//  Serial.print(", Y: ");
//  Serial.print(g.gyro.y);
//  Serial.print(", Z: ");
//  Serial.print(g.gyro.z);
//  Serial.println(" deg");
//
//  Serial.print("Temperature: ");
//  Serial.print(temp.temperature);
//  Serial.println(" degC");
//
//  Serial.println("");
//  delay(500);
}

//Distance Sensor
int sr04_getDistance() {
  long duration;
  int distance;
  // Clears the trigPin condition
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  // Displays the distance on the Serial Monitor

  return distance;
}

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
      if(get_BatteryVoltage()<=6.8){
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
      if(get_BatteryVoltage()<=6.8){
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
      if(get_BatteryVoltage()<=6.8){
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
      if(get_BatteryVoltage()<=6.8){
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