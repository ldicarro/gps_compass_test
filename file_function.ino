// --------------------------------------------------
// write file
void writeFile() {
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

  filePtr = SD.open("/log.txt", FILE_APPEND);
  if(filePtr) {
    Serial.println("file opened for write");
    filePtr.println(text);
    filePtr.close();
    Serial.println("file closed");
  }
  else {
    // if the file didn't open, print an error:
    Serial.println("error opening/write test.txt");
  }
}

void readFile() {
  filePtr = SD.open("/log.txt", FILE_READ);
  if(filePtr) {
    Serial.println("file opened for read");
    while(filePtr.available()) {
      Serial.write(filePtr.read());
    }
    filePtr.close();
    Serial.println("file closed");
  }
  else {
    // if the file didn't open, print an error:
    Serial.println("error opening/read test.txt");
  }
}