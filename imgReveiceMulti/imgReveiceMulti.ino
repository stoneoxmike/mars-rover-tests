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
  int ender = 0;                        //This counter will be used to check the last two bits of the file for EOF (FFD9)

  //Now we will receive 1k from the lander
  while(millis()-t < 450){              //Is this condition still needed with EOF check?  IF so, why 450?  -Bahn
    if(xBee.available() > 0) {
      byte b = xBee.read();             
      
      if (b == 217) ender++;            //Check for the final bit
      if (ender == 2) break;            //check if the final TWO bits have arrived
      ender = 0;                        //reset the EOF counter
      if (b == 255) ender++;            //Check if the second to last byte has arrived
      
      img.write(b);                     //MICHAEL, this should happen BEFORE the 4 lines for EOF checks above, yes?  Otherwise be don't write the last byte?
      i++;  
      Serial.print(i);
      Serial.print(", ");
      Serial.print(b);
      Serial.print(", ");
      Serial.println(ender);
      t = millis();                                   //reset t to start counting for delay again
    } else if((millis() - t) > 75) {                  //waits for delay of 1 second
        if((1000 - i) > 0 && (1000 - i) < 1000) {     //check that 1K bytes were written
          xBee.write("X");                            //sends back stuff if not
          Serial.println("error");
          img.seek(img.size() - i);
        }
      i = 0;
      t = millis();
      img.flush();          //store the received 1k of data to the SD card
    }
  }
  img.close();
  Serial.println("File Transfer REALLY Complete.");
}
