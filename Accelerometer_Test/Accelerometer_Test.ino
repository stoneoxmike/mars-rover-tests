/**************************************************************************/
/*!
    @file     Adafruit_MMA8451.h
    @author   K. Townsend (Adafruit Industries)
    @license  BSD (see license.txt)

    This is an example for the Adafruit MMA8451 Accel breakout board
    ----> https://www.adafruit.com/products/2019

    Adafruit invests time and resources providing this open source code,
    please support Adafruit and open-source hardware by purchasing
    products from Adafruit!

    @section  HISTORY

    v1.0  - First release
*/
/**************************************************************************/

#include <Wire.h>
#include <Adafruit_MMA8451.h>
#include <Adafruit_Sensor.h>

Adafruit_MMA8451 mma = Adafruit_MMA8451();

void setup(void) {
  Serial1.begin(9600);
  
  Serial1.println("Adafruit MMA8451 test!");
  

  if (! mma.begin()) {
    Serial1.println("Couldnt start");
    while (1);
  }
  Serial1.println("MMA8451 found!");
  
  mma.setRange(MMA8451_RANGE_2_G);
  
  Serial1.print("Range = "); Serial1.print(2 << mma.getRange());  
  Serial1.println("G");
  
}

void loop() {
  wait_for_acceleration(1.5);
}

void wait_for_acceleration(float threshold) {
  int i = 0;
  bool detected = false;
  while(!detected) {
    if(getXAcceleration() > threshold) i++;
      else i = 0;
    if(i >= 4) detected = true;
    delay(100);
    }
  if(detected == true) Serial1.println("DETECTED!");
}

float getXAcceleration() {
  mma.read();
  sensors_event_t event; 
  mma.getEvent(&event);
  return event.acceleration.x;
}

