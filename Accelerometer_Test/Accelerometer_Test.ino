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
  // Read the 'raw' data in 14-bit counts
  //mma.read();
  //Serial1.print("X:\t"); Serial1.print(mma.x); 
  //Serial1.print("\tY:\t"); Serial1.print(mma.y); 
  //Serial1.print("\tZ:\t"); Serial1.print(mma.z); 
  //Serial1.println();

  /* Get a new sensor event */ 
  //sensors_event_t event; 
  //mma.getEvent(&event);

  /* Display the results (acceleration is measured in m/s^2) */
  //Serial1.print("X: \t"); Serial1.print(event.acceleration.x); Serial1.print("\t");
  //Serial1.print("Y: \t"); Serial1.print(event.acceleration.y); Serial1.print("\t");
  //Serial1.print("Z: \t"); Serial1.print(event.acceleration.z); Serial1.print("\t");
  //Serial1.println("m/s^2 ");
  
  /* Get the orientation of the sensor */
  //uint8_t o = mma.getOrientation();
  /*
  switch (o) {
    case MMA8451_PL_PUF: 
      Serial.println("Portrait Up Front");
      break;
    case MMA8451_PL_PUB: 
      Serial.println("Portrait Up Back");
      break;    
    case MMA8451_PL_PDF: 
      Serial.println("Portrait Down Front");
      break;
    case MMA8451_PL_PDB: 
      Serial.println("Portrait Down Back");
      break;
    case MMA8451_PL_LRF: 
      Serial.println("Landscape Right Front");
      break;
    case MMA8451_PL_LRB: 
      Serial.println("Landscape Right Back");
      break;
    case MMA8451_PL_LLF: 
      Serial.println("Landscape Left Front");
      break;
    case MMA8451_PL_LLB: 
      Serial.println("Landscape Left Back");
      break;
    }*/
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

