void getData(){  
  if ( myPort.available() > 0) {  // If data is available,
    s_val[0] = myPort.read();         // read it and store it in val
    //println(s_val[0]);
  }

  int count=1;
  if (s_val[0]=='A') {
    while (count<=4) {
      while (myPort.available()==0) {
      }
      s_val[count] = myPort.read();
      count++;
    }
  }

  if (s_val[0]=='A' && s_val[4]=='B') {
    println("val0: "+ s_val[0] + "\t val1: " +s_val[1]+ "\t val2: " +s_val[2]+ "\t val3: " +s_val[3]+ "\t val4: " +s_val[4]);
    part_matter=s_val[1];
    ozone=s_val[2];
    tvoc=s_val[3];
  }
}