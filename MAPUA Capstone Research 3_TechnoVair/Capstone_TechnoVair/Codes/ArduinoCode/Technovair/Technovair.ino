/*
   Technovair Arduino Code
   04-27-2022
*/
#include <Wire.h>
#include <SPI.h>
#include <RF24.h>
#include <MQ131.h>
#include <LiquidCrystal_I2C.h>

#include <nRF24L01.h>
#include "PMS.h"
#include "SparkFun_SGP30_Arduino_Library.h"

#include <SoftwareSerial.h>

SoftwareSerial mySerial(19, 18); // RX, TX

LiquidCrystal_I2C lcd(0x27, 20, 4); // set the LCD address to 0x27 for a 16 chars and 2 line display

SGP30 mySensor; //create an object of the SGP30 class

RF24 radio(46, 47); // CE, CSN
const byte address[6] = "MCMTV";


PMS pms(Serial);
PMS::DATA data;


//Technovair data
unsigned long update_interval = 900000;
unsigned long prev_millis = 0;

int data_pm = 0;
double data_ozone = 0.0;
int data_tvoc = 0;


String radio_msg = " ";

#define FAN_pin 49

void setup()
{
  pinMode(FAN_pin, OUTPUT);
  //MYSERIAL
  mySerial.begin(9600);

  //PMS5003
  Serial.begin(9600);   // GPIO1, GPIO3 (TX/RX pin on ESP-12E Development Board)
  Serial1.begin(9600);  // GPIO2 (D4 pin on ESP-12E Development Board)

  //SGP30
  Wire.begin();
  //Initialize sensor
  if (mySensor.begin() == false) {
    Serial.println("No SGP30 Detected. Check connections.");
    while (1);
  }


  // Init the sensor
  // - Heater control on pin 2
  // - Sensor analog read on pin A0
  // - Model LOW_CONCENTRATION
  MQ131.begin(2, A0, LOW_CONCENTRATION, 1000000);
  Serial.print(MQ131.getR0());
  Serial.print(MQ131.getTimeToRead());


  //Initializes sensor for air quality readings
  //measureAirQuality should be called in one second increments after a call to initAirQuality
  mySensor.initAirQuality();

  //NRF24L01
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MAX);
  radio.stopListening();

  lcd.init();


  prev_millis == millis();
}

void loop() {

  if ((millis() - prev_millis) == update_interval) {
    while (!data_update()) {
      data_update();
    }
  }

  //Print the Particulate Matter Value on the LCD
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("PM:");
  lcd.print(data_pm);
  lcd.print(" uG/M3");

  //Print Alert Level for Particulate Matter
  lcd.setCursor(12, 0);
  lcd.print("AL:");
  lcd.print(getAlertLevel_PM(data_pm));

  //Print the Ozone Value on the LCD
  lcd.setCursor(0, 1);
  lcd.print("Ozone:");
  lcd.print(data_ozone);
  lcd.print(" PPM");
  //Print Alert Level for Ozone
  lcd.setCursor(12, 1);
  lcd.print("AL:");
  lcd.print(getAlertLevel_ozone(data_ozone));

  //Print the TVOC Value on the LCD
  lcd.setCursor(0, 1);
  lcd.print("TVOC:");
  lcd.print(data_tvoc);
  lcd.print(" PPB");
  //Print Alert Level for TVOC
  lcd.setCursor(12, 2);
  lcd.print("AL:");
  lcd.print(getAlertLevel_TVOC(data_tvoc));


  //Display the overall alert and  filter activity
  if (!getAlertLevel_TVOC(data_tvoc)) {
    if (getAlertLevel_PM(data_pm) < 2 && getAlertLevel_ozone(data_ozone) < 2) {
      lcd.setCursor(0, 3);
      lcd.print("FAIR  ");
      lcd.print("F:Disabled");
      digitalWrite(FAN_pin, LOW); //TURN OFF THE FAN (FILTER)
    } else if (getAlertLevel_PM(data_pm) >= 2 || getAlertLevel_ozone(data_ozone) >= 2) {
      lcd.setCursor(0, 3);
      lcd.print("TOXIC  ");
      lcd.print("F:Active");
      digitalWrite(FAN_pin, HIGH); //TURN ON THE FAN (FILTER)
    }
  } else {
    if (getAlertLevel_PM(data_pm) >= 2 || getAlertLevel_ozone(data_ozone) >= 2) {
      lcd.setCursor(0, 3);
      lcd.print("TOXIC  ");
      lcd.print("F:Active");
      digitalWrite(FAN_pin, HIGH); //TURN ON THE FAN (FILTER)
    }
  }

  //SEND THE DATA TO THE PC APP
  radio_msg = "A" + String(data_pm) + "," +  String(data_ozone) + "," + String(data_tvoc);
  radio.write(&radio_msg, sizeof(radio_msg));

  //SEND THE DATA TO THE PHONE APP
  mySerial.println("A" + String(data_pm) + "," +  String(data_ozone) + "," + String(data_tvoc));
  delay(3000);

}


bool data_update() {

  bool pms_updated = false;

  //PMS5003 UPDATE DATA
  if (pms.read(data))
  {
    Serial1.print("PM 1.0 (ug/m3): ");
    Serial1.println(data.PM_AE_UG_1_0);

    Serial1.print("PM 2.5 (ug/m3): ");
    Serial1.println(data.PM_AE_UG_2_5);

    Serial1.print("PM 10.0 (ug/m3): ");
    Serial1.println(data.PM_AE_UG_10_0);

    Serial1.println();
    data_pm = data.PM_AE_UG_1_0 + data.PM_AE_UG_2_5 + data.PM_AE_UG_10_0;
    pms_updated = true;
  }

  //MQ131 UPDATE DATA
  Serial.print(MQ131.getO3(PPB));
  Serial.println(" ppb");
  data_ozone = MQ131.getO3(PPB);

  //SGP30 UPDATE DATA
  mySensor.measureAirQuality();
  Serial.print("CO2: ");
  Serial.print(mySensor.CO2);
  Serial.print(" ppm\tTVOC: ");
  Serial.print(mySensor.TVOC);
  Serial.println(" ppb");
  data_tvoc = mySensor.TVOC;

  return pms_updated;
}
