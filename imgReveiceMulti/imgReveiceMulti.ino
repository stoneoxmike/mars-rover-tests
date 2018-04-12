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
  int eof = 0;
  byte packet[100];

  while(millis()-t < 450){ //the lander is transmitting 1K
    if(xBee.available() > 0) {
      packet[i] = xBee.read();
      
      if (packet[i] == 217) eof++;
      if (eof == 2) break; 
      eof = 0;
      if (packet[i] == 255) eof++;
            
      i++;  
      Serial.print(i);
      Serial.print(",    ");
      Serial.print(packet[i]);
      Serial.print(",    ");
      Serial.print(eof);
      Serial.print(",    ");
      t = millis();                  //reset t to start counting for delay again
    } else if((millis() - t) > 40) {          //waits for delay of 1 second
        if((100 - i) > 0 && (100 - i) < 100) {         //check that 1K bytes were written
          xBee.write("0"); //sends back zero if not
          Serial.println("error");  
        }
        else if((100 - i) == 0) {
          xBee.write("1");
          for(int k = 0; k < 100; k++){
            img.write(packet[k]);
            delay(8);  
          }
        }
      i = 0;
      t = millis();
      img.flush();
    }
  }
  img.close();
  Serial.println("File Transfer REALLY Complete.");
}
