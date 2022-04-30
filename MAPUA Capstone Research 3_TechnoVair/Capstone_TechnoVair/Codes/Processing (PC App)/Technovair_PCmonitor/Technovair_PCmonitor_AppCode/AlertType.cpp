
int getAlertLevel_PM(int PM_value){
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

int getAlertLevel_ozone(float ozone_value){
  if(ozone_value>=0 && ozone_value<0.065){
    return 0;
  }else if(ozone_value>=0.065 && ozone_value<0.085){
    return 1;
  }else if(ozone_value>=0.085 && ozone_value<0.105){
    return 2;
  }else if(ozone_value>=0.105 && ozone_value<0.125){
    return 3;
  }else if(ozone_value>=0.125 && ozone_value<0.375){
    return 4;
  }else if(ozone_value>=0.375){
    return 5;
  }
  return 0;
}

boolean getAlertLevel_TVOC(int tvoc_value){
  if(tvoc_value>=500){
    return true;
  }
  return false;
}