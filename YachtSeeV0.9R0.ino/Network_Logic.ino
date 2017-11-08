//***************************************************************************************
// YACHTSEE NETWORK LOGIC
//***************************************************************************************

void transmitData(String x){ // Transmits a character array < 100 char to thingsboard server
 
 // Just debug messages
  Serial.print("Sending the following String to server: "); //DEBUG
  Serial.println(x); //DEBUG
  
  // Send payload
  char attributes[100];
  x.toCharArray( attributes, 100 );
  yield();
  client.publish( "v1/devices/me/telemetry", attributes );
  //Serial.println( attributes );

  Serial.println("String sent successfully.");//DEBUG
  Serial.println();//DEBUG

} //transmitData

//*************************************************************************************** ADDDATA

void addData(String x){
  jsonPayload += x;
  yield();
} //addData

//*************************************************************************************** TIMEFORMATTER

String timeFormatter(String x){
  
  yield();
  
  String timeFormatted = "";
  String hours = "";
  String minutes = "";
  String seconds = "";
  
  hours = x.substring(0,2);
  minutes = x.substring(2,4);
  seconds = x.substring(4,6);

  timeFormatted += hours;
  //timeFormatted += ".";
  timeFormatted += minutes;
 // timeFormatted += ".";
  timeFormatted += seconds; 
 // timeFormatted += " GMT";
  ESP.wdtFeed(); 
  return timeFormatted;
} //timeFormatter

//*************************************************************************************** RECONNECT

void reconnect() {
  failCount = 0;
  // Loop until we're reconnected
  while (!client.connected() && failCount <= 7) {
    Serial.print("Connecting to dashboard server... ");//DEBUG
    // Attempt to connect (clientId, username, password)
    if ( client.connect("ESPSENSE", TOKEN, NULL) ) {
      Serial.println( "[DONE]" );//DEBUG
      yield();
    } //if(in while) 
    
    else {
      Serial.print( "[FAILED] [ rc = " );//DEBUG
      Serial.print( client.state() );
      Serial.println( " : retrying in 1 second]" );//DEBUG
      failCount++;
      delay(1000);
    } //else(in while)
    
    yield();
    
  } //while
  if (failCount >=7){
    ESP.restart();
  }
} //reconnect

//*************************************************************************************** CONFIGMODECALLBACK

void configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());
  //if you used auto generated SSID, print it
  Serial.println(myWiFiManager->getConfigPortalSSID());
}//configModeCallback

//*************************************************************************************** GETWIFISIG

String getWifiSig() {
  int32_t dbm = WiFi.RSSI();
  return String(dbm);
 
}//getWifiSig

