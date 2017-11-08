//***************************************************************************************
//YACHTSEE MAIN 
//*************************************************************************************** SERVERLOOP

class ServerLoop : public Task {
public:

  void loop(){
  //Serial.println("Running network check.. ");
  delay (20);
  if ( !client.connected() ) {
        Serial.println("Server disconnected, attempting to connect now..");
        reconnect();
      } //if
  //else { Serial.println("Server connected");}
  if ( millis() - lastSend > 1000 ) { // Update and send only after 1 seconds
        lastSend = millis();
      } //if
      client.loop(); 
  } //loop 
}server_loop;

//*************************************************************************************** MAINLOOP

class MainLoop : public Task{
public:
void loop(){
  
  jsonPayload = "{";
  
  jsonPayload += getGPS();
  delay(50);

  jsonPayload += getFloatSwitch();
  delay(50);

  jsonPayload += getADC();
  delay(50);
  
  jsonPayload += "}";
  
  transmitData(jsonPayload);
  delay(50);
  
  Serial.println(jsonPayload.length());
  delay (3000);
  
  ESP.wdtFeed();

  yield();
  client.loop(); 
  
} //loop

}main_loop;

//*************************************************************************************** SETUP

void setup(){
 Serial.begin(115200); 
 
 WiFiManager wifiManager;
 
 wifiManager.setConfigPortalTimeout(180);
 wifiManager.setAPCallback(configModeCallback);
 
 if (ESP.getResetReason() == "External System") {
       wifiManager.startConfigPortal(AP_NAME);
 }
 
 String hostname(HOSTNAME);
 hostname += String(ESP.getChipId(), HEX);
 WiFi.hostname(hostname);

Serial.println("HTTP TIMEOUT");

 // Setup OTA
 Serial.println("Hostname: " + hostname);
 ArduinoOTA.setHostname((const char *)hostname.c_str());
 ArduinoOTA.begin();


 pinMode (LOW_FLT_SW, INPUT_PULLUP);
 pinMode (HI_FLT_SW, INPUT_PULLUP);
 pinMode (0, OUTPUT); digitalWrite(0, HIGH); // set chip to reboot into sketch on crash
 pinMode (2, OUTPUT); digitalWrite(2, HIGH); // ""                             ""
 //pinMode (15, OUTPUT); digitalWrite(15, LOW); //GPIO 15 RESERVED FOR SOFTWARE SERIAL RX

 
 Serial.println("Starting up..");
 gpsSerial.begin(9600); // start serial communication between ESP8266 and NEO-6M-0-001
 ads.begin();
 
 
 client.setServer(thingsBoardServer, 1883);
 lastSend = 0;

 
 Scheduler.start(&server_loop);
 Scheduler.start(&main_loop);
 Scheduler.begin();
 
 
 Serial.println("Setup finished");
} //setup

//*************************************************************************************** LOOP

void loop() {
      ArduinoOTA.handle();
      yield();
      client.loop(); 
      yield();
} //loop

//***************************************************************************************
