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
  boolean xbeeAvailable = 0;

  while(millis()-t < 4500){ //the lander is transmitting 1K
    if(xBee.available() > 0) {
      byte b = xBee.read();
      img.write(b);
      //img.flush();
      i++;  
      Serial.println(i);

      if(millis() - t > 1000) {          //waits for delay of 1 second
        if(1000 - i != 0) {         //check that 1K bytes were written
          xBee.write("ERROR, RESEND"); //sends back stuff if not
          img.seek(img.size() - i);
          delay(100);
        }
        
        i = 0;
        Serial.print("i is ");
        Serial.println(i);
      }
      t = millis();                  //reset t to start counting for delay again
    }
  }
  img.close();
  Serial.println("File Transfer REALLY Complete.");
}
