#include <Wire.h>
#include <QMC5883LCompass.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <TinyGPSPlus.h>
#include "mainpage.h"

// Compass
QMC5883LCompass compass;

// Web Server
//provide your own WiFi SSID and password
const char* ssid = "BrapDeco";
const char* password = "cleanbrain303";

WebServer server(80);

// GPS Module
// Define pins for serial 2
#define RXD2 16
#define TXD2 17
#define GPS_BAUD 9600

TinyGPSPlus gps;
HardwareSerial gpsSerial(2); // create instance of HW serial for GPS

// get the reading from the compass and return the value
float getHeading() {
  delay(100);
  /* Get a new sensor event */ 
  compass.read();

  byte az = compass.getAzimuth();
  byte bearing = compass.getBearing(az); // 0 - 11

  return bearing;
}

// This custom version of delay() ensures that the gps object
// is being "fed".
static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
    while (gpsSerial.available())
      gps.encode(gpsSerial.read());
  } while (millis() - start < ms);
}


void setup(void) 
{
  Serial.begin(115200);
  
  /* Initialize the compass */
  compass.init();
  compass.setCalibrationOffsets(-290.00, -139.00, -813.00);
  compass.setCalibrationScales(0.98, 1.64, 0.73);

  /* Initialize the GPS module */
  gpsSerial.begin(GPS_BAUD, SERIAL_8N1, RXD2, TXD2);

  /* Initialize the web server */
  //Use ESP32 as WiFi Station
  WiFi.mode(WIFI_STA);
  //Initiate WiFi Connection
  WiFi.begin(ssid, password);
  Serial.println("");
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  //Print your WiFi's SSID (might be insecure)
  Serial.println(ssid);
  Serial.print("IP address: ");
  //Print your local IP address (needed for browsing the app)
  Serial.println(WiFi.localIP());
   //Home page. Contents of 'page' is in mainpage.h
  server.on("/", []() {
   server.send(200, "text/html", page);
  });
  //Page for reading data. Sensor is read in this part
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
  //start web server
  server.begin();
  //Just stating things
  Serial.println("HTTP server started");
}

void loop(void) 
{
  //Make the ESP32 always handle web clients
  server.handleClient();

  delay(1000);
}