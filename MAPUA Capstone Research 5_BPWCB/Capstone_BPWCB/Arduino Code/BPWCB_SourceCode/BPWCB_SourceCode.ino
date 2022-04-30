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
#define start_button 53


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
char* text;
char* number;
bool error; //to catch the response of sendSms

//GPS Variables
float gps_Lat;
float gps_Long;

//Gyroscope
int gyroX;
int gyroY;


//Start Button
bool started=false;

//Rudder(Servo) Variables
int rudder_pos=0;


//###############################################




void setup() {
  //Start Button
  pinMode(start_button, INPUT);

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
  text = "Testing Sms"; //text for the message.
  number = "09286707466"; //change to a valid number.
  error = Sim800l.sendSms(number, text);
  // OR
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


}

bool state_startbutton() {
  return digitalRead(start_button);
}


void getLocation(){
  while(Serial1.available()){
    if(gps.encode(Serial1.read())){
      gps.f_get_position(&gps_Lat, &gps_Long);
    }
  }
}
