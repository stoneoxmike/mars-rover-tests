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
  Serial.print("Waiting for xBee connection...");
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
  int eof = 0;  //end of file signifier
  byte packet[100];  //array of packet bytes
  unsigned int r = 1;  //packet counter
  int c = 1;  //file counter

  while(millis()-t < 450){ //the lander is transmitting the file
    if(xBee.available() > 0) {
      packet[i] = xBee.read();  //read xbee data to next position of packet

      //check for the end of file signature
      if (packet[i] == 217) eof++;  
      if (eof == 2) break; 
      eof = 0;
      if (packet[i] == 255) eof++;
            
      i++;

      if(i%100 == 0) {  //send serial info every 100 bytes
        Serial.print(r);
        Serial.print(" packet(s) recieved, last byte contains ");
        Serial.print(packet[0]);
        Serial.print(", around ");
        Serial.print(490 - r);
        Serial.print(" packets left ");
        Serial.print(c);
        Serial.print(", packet written: ");
      }
      
      t = millis();                  //reset t to start counting for delay again
    } 
    else if((millis() - t) > 40) {          //waits for delay of 1 second
      if((100 - i) > 0 && (100 - i) < 100) {         //check that 100 bytes were written
        xBee.write(48); //sends back zero if not
        Serial.println("ERROR: packet not complete, retrying");  
      }
      else if((100 - i) == 0) {   //if packet is full, write to the SD card and confirm
        xBee.write(49);
        for(int k = 0; k < 100; k++) {
          img.write(packet[k]); 
        }
        r++;  //increment packet counter
        Serial.println("check");
        img.flush();
      }
      i = 0;          //reset counters
      t = millis();
    }
  }
  Serial.println("Writing Final Packet");
  for(int k = 0; k < (i + 1); k++) {      //Must right final packet before closing...i is +1 because we break before we do i++
    img.write(packet[k]); 
  }
  img.close();
  c++;  //increment file counter
  Serial.println("File Transfer Complete.");
}
