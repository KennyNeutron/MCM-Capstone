//Technovair PC Monitor app

PImage img_backdrop;

PFont font_bebasneue;
PFont font_regularBold;
PFont font_regular;
PFont font_regularcontent;

int part_matter=98;
float ozone= 0.041;
int tvoc= 131;

int updateTime= 15000;
int prevmillis=0;

boolean flag_dataUpdate=false;

void setup() {
  img_backdrop= loadImage("bg.jpg");

  font_regularBold = createFont("regularBold.ttf", 36);
  font_bebasneue = createFont("BebasNeue-Regular.ttf", 36);
  font_regularcontent= createFont("square_721_bt.ttf", 36);
  size(720, 480);
  background(255);
  prevmillis=millis();
}


void draw() {
  background(255);
  image(img_backdrop, 0, 0);

  header();
  measuredValues();


  if (flag_dataUpdate) {

    part_matter=int(random(0, 1500));
    ozone= random(0.000, 0.450);
    tvoc= int(random(0, 800));

    flag_dataUpdate=!flag_dataUpdate;
    prevmillis= millis();
  } else {
    if (millis()-prevmillis>=updateTime) {
      flag_dataUpdate=true;
    }
  }

  println(millis());
}


void header() {
  fill(255);
  stroke(2);
  rectMode(RADIUS);
  rect(width/2, 50, 200, 40, 15);

  stroke(0);
  fill(0);
  textAlign(CENTER, CENTER);
  textFont(font_regularBold);
  textSize(30);
  text("TECHNOVAIR PC Monitor", width/2, 50);
}

void measuredValues() {
  strokeWeight(2);
  fill(255);
  rectMode(CORNER);
  rect(5, 170, 470, 40, 15);

  rect(5, 218, 470, 40, 15);

  rect(5, 268, 470, 40, 15);

  stroke(0);
  fill(0);
  textAlign(TOP, LEFT);
  textFont(font_regularcontent);

  textSize(28);
  text("Particulate Matter: "+str(part_matter)+" uG/M3", 10, 200);

  String PM_description="";
  switch(getAlertLevel_PM(part_matter)) {
  case 0:
    fill(#00ff00);
    PM_description="GOOD";
    rect(495, 170, 70, 40, 15);
    break;
  case 1:
    fill(#0066ff);
    PM_description="FAIR";
    rect(495, 170, 55, 40, 15);
    break;  
  case 2:
    fill(#66ffff);
    PM_description="UNHEALTHY";
    rect(495, 170, 130, 40, 15);
    break;
  case 3:
    fill(#ffff00);
    PM_description="VERY UNHEALTHY";
    rect(495, 170, 190, 40, 15);
    break;
  case 4:
    fill(#ff9900);
    PM_description="ACUTELY UNHEALTHY";
    rect(495, 170, 220, 40, 15);
    break;
  case 5:
    fill(#ff0000);
    PM_description="EMERGENCY";
    rect(495, 170, 140, 40, 15);
    break;
  }

  fill(0);
  textSize(20);
  text(PM_description, 500, 200);




  fill(0);
  textSize(30);
  text("Ozone:  " +  str((float)ozone) + " PPM", 10, 250); 

  String ozone_description="";
  switch(getAlertLevel_ozone(ozone)) {
  case 0:
    fill(#00ff00);
    ozone_description="GOOD";
    rect(495, 218, 70, 40, 15);
    break;
  case 1:
    fill(#0066ff);
    ozone_description="FAIR";
    rect(495, 218, 55, 40, 15);
    break;  
  case 2:
    fill(#66ffff);
    ozone_description="UNHEALTHY";
    rect(495, 218, 130, 40, 15);
    break;
  case 3:
    fill(#ffff00);
    ozone_description="VERY UNHEALTHY";
    rect(495, 218, 190, 40, 15);
    break;
  case 4:
    fill(#ff9900);
    ozone_description="ACUTELY UNHEALTHY";
    rect(495, 218, 220, 40, 15);
    break;
  case 5:
    fill(#ff0000);
    ozone_description="EMERGENCY";
    rect(495, 218, 140, 40, 15);
    break;
  }
  fill(0);
  textSize(20);
  text(ozone_description, 500, 250);


  String tvoc_description="";
  if (getAlertLevel_TVOC(tvoc)) {
    fill(#ff0000);
    tvoc_description="EMERGENCY";
    rect(495, 268, 140, 40, 15);
  } else {
    fill(#00ff00);
    tvoc_description="GOOD";
    rect(495, 268, 70, 40, 15);
  }

  fill(0);
  textSize(20);
  text(tvoc_description, 500, 300);

  fill(0);
  textSize(30);
  text("TVOC: " + str((float)tvoc) + " PPB", 10, 300);


  fill(0);
  textSize(30);
  text("ALERT:", 10, 400);
  text("FILTRATION:", 10, 440);

  if (getAlertLevel_TVOC(tvoc)) {
    fill(#FF0000);
    textFont(font_regularBold);
    text("TOXIC", 130, 400);

    fill(#0000FF);
    textFont(font_regularBold);
    text("ACTIVE", 190, 440);
  } else {
    if (getAlertLevel_PM(part_matter)>1 || getAlertLevel_ozone(ozone)>1 ) {
      fill(#FF0000);
      textFont(font_regularBold);
      text("TOXIC", 130, 400);

      fill(#0000FF);
      textFont(font_regularBold);
      text("ACTIVE", 190, 440);
    } else {
      fill(#00FF00);
      textFont(font_regularBold);
      text("FAIR", 130, 400);

      fill(#ff6600);
      textFont(font_regularBold);
      text("DISABLED", 190, 440);
    }
  }
}


void keyPressed() {
  if (key=='r' || key=='R') {
    part_matter=97;
    ozone= 0.048;
    tvoc= 149;
  } else if (key=='1') {
    part_matter=232;
  } else if (key=='2') {
    ozone=0.097;
  } else if (key=='3') {
    tvoc= 599;
  }

  prevmillis=millis();
}
