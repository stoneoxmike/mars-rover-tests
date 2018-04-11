#include <SPI.h>
#include <SD.h>
#include <SoftwareSerial.h>

SoftwareSerial xBee(5, 6); // RX, TX



File img;

void setup() {
  Serial.begin(115200);
  xBee.begin(9600);

  Serial.print("Initializing SD card...");

  if (!SD.begin(10)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
}

void loop() {
  while(xBee.available() == 0);   //wait for data to begin streaming in
  Serial.println("xBee data is available");
  
  char filename[13];      // Create an image with the name IMAGExx.JPG
  strcpy(filename, "IMAGE00.JPG");
  
  for (int i = 0; i < 100; i++) {
    filename[5] = '0' + i/10;
    filename[6] = '0' + i%10;
    // create if does not exist, do not open existing, write, sync after write
    if (!SD.exists(filename)) {
      break;
    }
  }

  unsigned int i = 0;
  img = SD.open(filename, FILE_WRITE);
  Serial.print("Opening Image File: ");
  Serial.println(filename);
  unsigned long t = millis();
  int ender = 0;

  while(millis()-t < 450){ //the lander is transmitting 1K
    if(xBee.available() > 0) {
      byte b = xBee.read();
      if (b == 217) ender++;
      if (ender == 2) break; 
      ender = 0;
      if (b == 255) ender++;
      
      img.write(b);
      i++;  
      Serial.print(i);
      Serial.print(", ");
      Serial.print(b);
      Serial.print(", ");
      Serial.println(ender);
      t = millis();                  //reset t to start counting for delay again
    } else if((millis() - t) > 75) {          //waits for delay of 1 second
        if((1000 - i) > 0 && (1000 - i) < 1000) {         //check that 1K bytes were written
          xBee.write("X"); //sends back stuff if not
          Serial.println("error");
          img.seek(img.size() - i);
        }
      i = 0;
      t = millis();
      img.flush();
    }
  }
  img.close();
  Serial.println("File Transfer REALLY Complete.");
}
