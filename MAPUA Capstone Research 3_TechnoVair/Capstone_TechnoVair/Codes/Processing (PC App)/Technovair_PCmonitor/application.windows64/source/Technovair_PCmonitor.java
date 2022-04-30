import processing.core.*; 
import processing.data.*; 
import processing.event.*; 
import processing.opengl.*; 

import java.util.HashMap; 
import java.util.ArrayList; 
import java.io.File; 
import java.io.BufferedReader; 
import java.io.PrintWriter; 
import java.io.InputStream; 
import java.io.OutputStream; 
import java.io.IOException; 

public class Technovair_PCmonitor extends PApplet {

//Technovair PC Monitor app

PImage img_backdrop;

PFont font_bebasneue;
PFont font_regularBold;
PFont font_regular;
PFont font_regularcontent;

int part_matter=98;
float ozone= 0.041f;
int tvoc= 131;

int updateTime= 15000;
int prevmillis=0;

boolean flag_dataUpdate=false;

public void setup() {
  img_backdrop= loadImage("bg.jpg");

  font_regularBold = createFont("regularBold.ttf", 36);
  font_bebasneue = createFont("BebasNeue-Regular.ttf", 36);
  font_regularcontent= createFont("square_721_bt.ttf", 36);
  
  background(255);
  prevmillis=millis();
}


public void draw() {
  background(255);
  image(img_backdrop, 0, 0);

  header();
  measuredValues();


  if (flag_dataUpdate) {

    part_matter=PApplet.parseInt(random(0, 1500));
    ozone= random(0.000f, 0.450f);
    tvoc= PApplet.parseInt(random(0, 800));

    flag_dataUpdate=!flag_dataUpdate;
    prevmillis= millis();
  } else {
    if (millis()-prevmillis>=updateTime) {
      flag_dataUpdate=true;
    }
  }

  println(millis());
}


public void header() {
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

public void measuredValues() {
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
    fill(0xff00ff00);
    PM_description="GOOD";
    rect(495, 170, 70, 40, 15);
    break;
  case 1:
    fill(0xff0066ff);
    PM_description="FAIR";
    rect(495, 170, 55, 40, 15);
    break;  
  case 2:
    fill(0xff66ffff);
    PM_description="UNHEALTHY";
    rect(495, 170, 130, 40, 15);
    break;
  case 3:
    fill(0xffffff00);
    PM_description="VERY UNHEALTHY";
    rect(495, 170, 190, 40, 15);
    break;
  case 4:
    fill(0xffff9900);
    PM_description="ACUTELY UNHEALTHY";
    rect(495, 170, 220, 40, 15);
    break;
  case 5:
    fill(0xffff0000);
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
    fill(0xff00ff00);
    ozone_description="GOOD";
    rect(495, 218, 70, 40, 15);
    break;
  case 1:
    fill(0xff0066ff);
    ozone_description="FAIR";
    rect(495, 218, 55, 40, 15);
    break;  
  case 2:
    fill(0xff66ffff);
    ozone_description="UNHEALTHY";
    rect(495, 218, 130, 40, 15);
    break;
  case 3:
    fill(0xffffff00);
    ozone_description="VERY UNHEALTHY";
    rect(495, 218, 190, 40, 15);
    break;
  case 4:
    fill(0xffff9900);
    ozone_description="ACUTELY UNHEALTHY";
    rect(495, 218, 220, 40, 15);
    break;
  case 5:
    fill(0xffff0000);
    ozone_description="EMERGENCY";
    rect(495, 218, 140, 40, 15);
    break;
  }
  fill(0);
  textSize(20);
  text(ozone_description, 500, 250);


  String tvoc_description="";
  if (getAlertLevel_TVOC(tvoc)) {
    fill(0xffff0000);
    tvoc_description="EMERGENCY";
    rect(495, 268, 140, 40, 15);
  } else {
    fill(0xff00ff00);
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
    fill(0xffFF0000);
    textFont(font_regularBold);
    text("TOXIC", 130, 400);

    fill(0xff0000FF);
    textFont(font_regularBold);
    text("ACTIVE", 190, 440);
  } else {
    if (getAlertLevel_PM(part_matter)>1 || getAlertLevel_ozone(ozone)>1 ) {
      fill(0xffFF0000);
      textFont(font_regularBold);
      text("TOXIC", 130, 400);

      fill(0xff0000FF);
      textFont(font_regularBold);
      text("ACTIVE", 190, 440);
    } else {
      fill(0xff00FF00);
      textFont(font_regularBold);
      text("FAIR", 130, 400);

      fill(0xffff6600);
      textFont(font_regularBold);
      text("DISABLED", 190, 440);
    }
  }
}


public void keyPressed() {
  if (key=='r' || key=='R') {
    part_matter=97;
    ozone= 0.048f;
    tvoc= 149;
  } else if (key=='1') {
    part_matter=232;
  } else if (key=='2') {
    ozone=0.097f;
  } else if (key=='3') {
    tvoc= 599;
  }

  prevmillis=millis();
}


public int getAlertLevel_PM(int PM_value){
  if(PM_value>=0 && PM_value<80){
    return 0;
  }else if(PM_value>=80 && PM_value<230){
    return 1;
  }else if(PM_value>=230 && PM_value<350){
    return 2;
  }else if(PM_value>=350 && PM_value<600){
    return 3;
  }else if(PM_value>=600 && PM_value<900){
    return 4;
  }else if(PM_value>=900){
    return 5;
  }
  
  return 0;
}

public int getAlertLevel_ozone(float ozone_value){
  if(ozone_value>=0 && ozone_value<0.065f){
    return 0;
  }else if(ozone_value>=0.065f && ozone_value<0.085f){
    return 1;
  }else if(ozone_value>=0.085f && ozone_value<0.105f){
    return 2;
  }else if(ozone_value>=0.105f && ozone_value<0.125f){
    return 3;
  }else if(ozone_value>=0.125f && ozone_value<0.375f){
    return 4;
  }else if(ozone_value>=0.375f){
    return 5;
  }
  return 0;
}

public boolean getAlertLevel_TVOC(int tvoc_value){
  if(tvoc_value>=500){
    return true;
  }
  return false;
}
  public void settings() {  size(720, 480); }
  static public void main(String[] passedArgs) {
    String[] appletArgs = new String[] { "Technovair_PCmonitor" };
    if (passedArgs != null) {
      PApplet.main(concat(appletArgs, passedArgs));
    } else {
      PApplet.main(appletArgs);
    }
  }
}
