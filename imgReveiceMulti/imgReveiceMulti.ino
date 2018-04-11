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

  while(xBee.available() > 0){ //the lander is transmitting 1K
    byte b = xBee.read();
    img.write(b);
    img.flush();
    i++;  
    t = millis();

    if(millis() - t > 1000) {          //waits for delay of 1 second
      if(millis() - t < 5000) {        //make sure there isn't a delay of 5 seconds
        if(img.size - i > 0) {         //check that 1K bytes were written
          xBee.write("ERROR, RESEND"); //sends back stuff if not
        }
        t = millis();                  //reset t to start counting for delay again
      } else {                         //if there is a delay of 5 seconds, end reception
        img.close();
        Serial.println("File Transfer REALLY Complete.");
      }
    }
  }
}
