// get the reading from the compass and return the value
float getHeading() {
  delay(100);
  /* Get a new sensor event */ 
  compass.read();

  byte az = compass.getAzimuth();
  byte bearing = compass.getBearing(az); // 0 - 11

  return bearing;
}