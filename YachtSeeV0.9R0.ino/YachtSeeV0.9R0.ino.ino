//***************************************************************************************

//YACHTSEE SENSOR MODULE SOFWARE V0.8R0
//CONTRIBUTIONS BY:
    //Clifford Smyth
    //James Smyth
    //Raoul Chapman

/**The MIT License (MIT)

Copyright (c) 2017 by James Smyth

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/
//*************************************************************************************** INCLUDES

#include <Arduino.h>
#include <Scheduler.h>
#include <Wire.h>
#include <TimeLib.h>
#include <ArduinoJson.h>
#include <Adafruit_ADS1015.h>
#include <SoftwareSerial.h> // Library that allows serial action local to the dev platform and connected device
#include <NMEA.h> // GPS library
#include <PubSubClient.h> // Json publishing library (mqtt)
#include <ESP8266WiFi.h> // Wifi library for ESP8266 development platform
#include <ArduinoOTA.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <ESP8266mDNS.h>
#include <DNSServer.h>
#include "TimeClient.h"

//*************************************************************************************** DEFINITIONS

#define HOSTNAME /*"LOOKOUT"*/ "ESP8266-OTA-"
#define AP_NAME "YACHTSEE_0.7"
#define AP_PASSWORD "yachtsea"
#define WIFI_PASSWORD "corporaklepto" //"g00dd0gh3rm10n3"
#define TOKEN "JwuprSgJzd3tK5Jt8A0A" //"fwszRY94uARN9N8oqWno" // device token for thingsboard

//*************************************************************************************** CONSTANTS

const float UTC_OFFSET = -4;

const int UPDATE_INTERVAL_SECS = 10 * 60; // Update every 10 minutes
const int LOW_FLT_SW = 12;
const int HI_FLT_SW = 14;
const int GPS_RX = 13;
const int SDC_PIN = 5;
const int SDA_PIN = 4;

//*************************************************************************************** GLOBALS

String getWifiSig();
String jsonPayload = "";

char thingsBoardServer[] = "142.44.211.112"; //"demo.thingsboard.io"; //"192.168.42.31"; // server address for thingsboard

int status = WL_IDLE_STATUS;
int failCount;

unsigned long lastSend;

//*************************************************************************************** INITIALIZATIONS AND PROTOTYPES

TimeClient timeClient(UTC_OFFSET);

SoftwareSerial gpsSerial(GPS_RX, 0); // setup software serial for pins 13(RX), and 0(TX) on the ESP8266 ) is unused, as the serial from the gps is listen only

//ADC
Adafruit_ADS1115 ads1115(0x48);  
Adafruit_ADS1115 ads;

//GPS
NMEA gps; 

//Networking
void configModeCallback (WiFiManager *myWiFiManager);
WiFiClient wifiClient;
PubSubClient client(wifiClient);

//!!!!!!!!!!!!!!!!!!!!!!!!!!!
// SETUP IN CONTROL_LOOPS TAB
//!!!!!!!!!!!!!!!!!!!!!!!!!!!

