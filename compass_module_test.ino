#include <Wire.h>
#include <QMC5883LCompass.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <TinyGPSPlus.h>
#include "mainpage.h"
#include <SPI.h>
#include <SD.h>

// --------------------------------------------------
// compass
QMC5883LCompass compass;

// --------------------------------------------------
// web Server
//provide your own WiFi SSID and password
const char* ssid = "BrapDeco";
const char* password = "cleanbrain303";

WebServer server(80);

// --------------------------------------------------
// GPS Module
// Define pins for serial 2
#define RXD2 16
#define TXD2 17
#define GPS_BAUD 9600

TinyGPSPlus gps;
HardwareSerial gpsSerial(2); // create instance of HW serial for GPS

// --------------------------------------------------
// SD card
File filePtr;

// --------------------------------------------------
// state timer
unsigned long startMillis;
unsigned long currentMillis;
const unsigned long period = 10000; 

// --------------------------------------------------
// SETUP
void setup(void) 
{
  Serial.begin(115200);

  // --------------------------------------------------
  // initialize the sd card
  if(!SD.begin(5)) {
    Serial.println("sd card initialization failed");
    while(1); // loop forever on error
  }
  Serial.println("sd card initialized");
  

  // --------------------------------------------------
  // initialize the compass
  compass.init();
  compass.setCalibrationOffsets(-290.00, -139.00, -813.00);
  compass.setCalibrationScales(0.98, 1.64, 0.73);

  // --------------------------------------------------
  // initialize the GPS module
  gpsSerial.begin(GPS_BAUD, SERIAL_8N1, RXD2, TXD2);

  // --------------------------------------------------
  // initialize the web server
  WiFi.mode(WIFI_STA);

  // initiate WiFi Connection
  WiFi.begin(ssid, password);
  Serial.println("");

  // wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // break out of while loop after connection
  Serial.println("");
  Serial.print("Connected to ");
  
  // print SSID (might be insecure)
  Serial.println(ssid);
  Serial.print("IP address: ");
  
  // print local IP address (needed for browsing the app)
  Serial.println(WiFi.localIP());
  
  // --------------------------------------------------
  // server event handlers
  // send home page. Contents of 'page' is in mainpage.h
  server.on("/", []() {
   server.send(200, "text/html", page);
  });

  // send data as json
  server.on("/data", [](){
    smartDelay(1000);

    String text = "";
  
    text += "{";
    text += "\"hdg\": " + String(getHeading()) + ",";
    text += "\"sat\": " + String(gps.satellites.value()) + ",";
    text += "\"lat\": " + String(gps.location.lat()) + ",";
    text += "\"lng\": " + String(gps.location.lng()) + ",";
    text += "\"alt\": " + String(gps.altitude.meters()) + ",";
    text += "\"dte\": \"" + String(gps.date.month()) + "/" + String(gps.date.day()) + "/" + String(gps.date.year()) + "\",";
    text += "\"tme\": \"" + String(gps.time.hour()) + ":" + String(gps.time.minute()) + ":" + String(gps.time.second()) + "\",";
    text += "\"spd\": " + String(gps.speed.kmph());
    text += "}";

    server.send(200, "application/json", text);
  });

  // start web server
  server.begin();
  Serial.println("HTTP server started");

  // setup timer
  startMillis = millis();

}

// --------------------------------------------------
// LOOP
void loop(void) 
{
  // handle web clients
  server.handleClient();

  // write file every 10 seconds
  currentMillis = millis();
  if(currentMillis - startMillis >= period) {
    writeFile();
    startMillis = currentMillis;
  }
}