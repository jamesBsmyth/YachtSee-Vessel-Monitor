//***************************************************************************************
// YACHTSEE DATA_COLLECTION
//*************************************************************************************** GETADC

String getADC(){
  
  double volts = 0;
  double amps = 0;
  
  int motion = 0;
   
  String adcPayload = "";

  for (int x = 0;x<15;x++){
    volts += getVolts();
    amps += getBPAmps();
    motion += getMotion();
    delay(200); //(1000)
  }//forloop

  volts = volts / 15;
  amps = amps / 15;

  adcPayload += ", \"V\":";
  adcPayload += (volts);

  adcPayload += ", \"BPA\":";
  adcPayload += (amps);
  
  adcPayload += ", \"MotSen\":";
  adcPayload += motion;
 
  yield();
  
  ESP.wdtFeed();
  
  return adcPayload;
  
} //getADC

//*************************************************************************************** GETMOTION

int getMotion(){

  int adc2;

  //ads1115.setGain(GAIN_ONE);     // 1x gain   +/- 4.096V  1 bit = 2mV
  ads1115.setGain(GAIN_TWO);     // 2x gain   +/- 2.048V  1 bit = 1mV

  adc2 = ads1115.readADC_SingleEnded(2);

//  Serial.println(adc2); //DEBUG
  yield();
  ESP.wdtFeed();

  if (adc2 > 50 && adc2 < 40000) { 
    adc2 = 1;
  } //if 
  
  else{
    adc2 = 0;
  } //else
  
  return adc2;
  
} //getMotion

//*************************************************************************************** GETVOLTS

double getVolts(){
  //Serial.println("Collecting Volts."); //DEBUG

  double volts;

  int16_t adc0;

  //ads1115.setGain(GAIN_ONE);     // 1x gain   +/- 4.096V  1 bit = 2mV
  ads1115.setGain(GAIN_TWO);     // 2x gain   +/- 2.048V  1 bit = 1mV

  adc0 = ads1115.readADC_SingleEnded(0);

  volts = adc0;
  volts = volts*0.000685;
  //Serial.println(volts); //DEBUG
  
  yield();
  ESP.wdtFeed();
  return volts;
  
} //getVolts

//*************************************************************************************** GETBPAMPS

double getBPAmps(){
  //Serial.println("Collecting Amps."); //DEBUG

  double amps;

  int16_t adc1;

  //ads1115.setGain(GAIN_ONE);     // 1x gain   +/- 4.096V  1 bit = 2mV
  ads1115.setGain(GAIN_TWO);     // 2x gain   +/- 2.048V  1 bit = 1mV

  adc1 = ads1115.readADC_SingleEnded(1);
 
  amps = adc1;
 // amps = amps*0.76309;
  //Serial.println(amps); //DEBUG
  
  yield();
  ESP.wdtFeed();
  return amps;
  
} //getBPAmps

//*************************************************************************************** LATLONGFORMATTER

double latLongFormatter(String x){ // String x must be 11-12 characters long, and NMEA format 
  
  String minutesRaw = "";
  String decMRawStr = "";
  String degStr = "";
  String lenAdjust = "0";
  double degDec;
  double mFinal;
  double mRawDub;
  double multiplier;
  float degTemp;
  float mRawTemp;
  float decMRawTemp;
  char buffer[11];
  

  if (x.endsWith("N") || x.endsWith("E")){
    multiplier = 1;
  } //if
  
  else if(x.endsWith("S") || x.endsWith("W")){
    multiplier = -1;
  } //else if 
  
  else{
    Serial.println("ERROR, not NMEA format");
    return 0;
  } //else
  
  yield();
  
  //Serial.println("\n\n"); // DEBUG
  //Serial.print("String length: "); //DEBUG
  //Serial.println(x.length()); //DEBUG
  //Serial.println("\n\n"); //DEBUG
  
  if (x.length() == 11 || x.length() == 12){
    if (x.length() == 11){
     lenAdjust += x;
      x = lenAdjust;
     //Serial.println("String length adjusted to 12 characters for simple parsing"); // DEBUG
     //Serial.print("New string: "); // DEBUG
     //Serial.println(x); // DEBUG
     //Serial.println(); // DEBUG
   } //if(in if)
   
   else{
     //Serial.println("String length correct"); // DEBUG
     //Serial.print("It looks like this: "); // DEBUG
     //Serial.println(x); // DEBUG 
     //Serial.println(); // DEBUG  
   } //else(in if)
  } //if
  
  else{
    Serial.println("ERROR: string length incorrect"); // DEBUG
    //Serial.print("Failed to format string containing: "); // DEBUG
    //Serial.println(x); // DEBUG
    return 0;
  } //else

  if (x.length() != 12){
    Serial.println("ERROR: string length still wrong after adjustment"); // DEBUG
    //Serial.print("Failed to format string containing: "); // DEBUG
    //Serial.println(x); // DEBUG
    return 0;
  } //if
  
  else{
  //Serial.println("Formatting String data to double...."); // DEBUG
  degStr = x.substring(0,3);
  //Serial.println(degStr); // DEBUG
  degTemp = degStr.toFloat();
  //Serial.println(degTemp); // DEBUG
  minutesRaw = x.substring(3,5);
  //Serial.println(minutesRaw); // DEBUG
  mRawTemp = minutesRaw.toFloat();
  //Serial.println(mRawTemp); // DEBUG
  decMRawStr = x.substring(5,12);
  //Serial.println(decMRawStr); // DEBUG
  decMRawTemp = decMRawStr.toFloat();
  //Serial.println(decMRawTemp,5); // DEBUG
  degDec = degTemp;
  //Serial.println(degDec,5); // DEBUG
  mRawDub = (mRawTemp+decMRawTemp);
  //Serial.println(mRawDub,5); // DEBUG
  mFinal = (mRawDub/60);
  //Serial.println(mFinal,5); // DEBUG
  degDec += mFinal;
  degDec = (degDec*multiplier);
  yield();
  ESP.wdtFeed();
  } //else

  return degDec;

} //latLongFormatter

//*************************************************************************************** GETFLOATSWITCH

String getFloatSwitch() {
  
  byte floatStatus;
  
  String floatPayload;
  String hiStatStr;
  String lowStatStr;
  String warning;
  floatPayload = "";
  warning = "BILGE OK";

  if (digitalRead(LOW_FLT_SW) == HIGH && digitalRead(HI_FLT_SW) == HIGH){
     Serial.println("LOW AND HIGH SWITCHES TRIGGERED. WATER LEVEL DANGEROUS");
     warning = "DANGER! LOW AND HIGH SWITCHES TRIGGERED!";
     floatPayload = "\"BilStat\":";
     floatPayload += "2";
  } //if
  
  else if (digitalRead(LOW_FLT_SW) == HIGH){
    Serial.println("LOW FLOAT SWITCH TRIGGERED. WATER LEVEL WARNING");
    warning = "BILGE LEVEL HIGH";
    floatPayload = "\"BilStat\":";
    floatPayload += "1";
  } //else if
  
  else if (digitalRead(HI_FLT_SW) == HIGH){
    Serial.println("PROBLEM WITH LOW FLOAT SWITCH, HIGH SWITCH TRIGGERED: CHECK BILGE");
    warning = "PROBLEM WITH LOW FLOAT SWITCH, HIGH SWITCH TRIGGERED: CHECK BILGE";
    floatPayload = "\"BilStat\":";
    floatPayload += "3";
  } //else if
  
  else{
    Serial.println("BILGE OK");
    floatPayload = "\"BilStat\":";
    floatPayload += "0";
  } //else
  
  yield();
  ESP.wdtFeed();
  return floatPayload;    
} //getFloatSwitch

//*************************************************************************************** GETGPS

String getGPS(){
  
  String latiStr = "";
  String longStr = "";
  String latitude = "";
  String longitude = "";
  String gpsPayload = "";
  String gpsBuffer = "";
//  String dateTemp = "";
//  String dateFinal = "";
//  String monthStr = "";
//  String dayStr = "";
//  String yearStr = "";
//  String timeTemp = "";
//  String timeFinal = "";
//  String hourStr = "";
//  String minuteStr = "";
//  String secondStr = "";
  
  double latFinal;
  double lonFinal;
  
  char gpsData = 10; // define gps data as a carriage return
  
  gpsPayload = "";
  
  lastSend = millis();
  
  Serial.println("Trying to connect to gps");
  
  while(millis()-lastSend < 1500){
    
  yield();
  
  if(gpsSerial.available()){ // as long as there is a  serial connection between ESP and NEO
    //Serial.println("Reading gps buffer"); //DEBUG
    gpsData = gpsSerial.read(); // define gpsData as the values pulled from the SoftwareSerial connection between ESP and NEO 
    //Serial.write(gpsData); // DEBUG
    
    if (gps.addChar(gpsData)){ // try read gps data using the NMEA library and append it to gpsData  
    
     if (gps.validFix()){ // if the gps has a signal, do all this
      latitude = gps.getLatitude();
      yield();
      latFinal = latLongFormatter(latitude);
      longitude = gps.getLongitude();
      yield();
      lonFinal = latLongFormatter(longitude);
      latiStr = String(latFinal, 7);
      longStr = String(lonFinal, 7);
      gpsPayload = "\"lat\":";
      gpsPayload += latiStr;
      gpsPayload += ", \"long\":";
      gpsPayload += longStr;
      gpsPayload += ", ";
      //Serial.println(gpsPayload);  //DEBUG

      yield();

//      dateTemp = gps.getDate();
//      dayStr = dateTemp.substring(0,2);
//      monthStr = dateTemp.substring(2,4);
//      yearStr = dateTemp.substring(4,6);
//      gpsPayload += ", \"Day\": ";
//      gpsPayload += dayStr;
//      gpsPayload += ", \"Month\": ";
//      gpsPayload += monthStr;
//      gpsPayload += ", \"Year\": 20";
//      gpsPayload += yearStr;
//
//      yield();
//
//      timeTemp = gps.getTime();
//      gpsPayload += ", \"Time\": ";
//      timeFinal = timeFormatter(timeTemp);
//      gpsPayload += timeFinal;
      
       } //if(in if, in if, in while)
     } //if(in if, in while)
   } //if(in while)
   
   yield();
   ESP.wdtFeed();
   
 } //while
 
  return gpsPayload;
  
} //getGPS

//*************************************************************************************** 

