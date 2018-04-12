//Final Version of Program for Launch Day
//DO NOT CHANGE UNLESS APPROVED BY BAHN!
//This is NOT for testing subsystems.  This is a one-shot, here-we-go, final version.

#include <Servo.h>
#include <SD.h>
#include <SPI.h>
#include <Adafruit_MMA8451.h>
#include <Adafruit_VC0706.h>
#include <SoftwareSerial.h>
#include <SR04.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

#define FORWARD_TIME 5000
#define SPIN_TIME 350
#define TURRET_TIME 3500

Servo parachute_servo;        //Initialize PARACHUTE Servo
Servo tankRelease_servo;      //Initialize TANK Release Servo

Adafruit_MMA8451 mma;         //Create variable mma to be referenced later by other functions

SR04 sr04  = SR04(6, 7);      //Initialize UTS and name it "sr04"  (Echo_pin = 6, Trig_pin = 7)
  
String imageFileNames[4]; // This is an array of the file names of the images that were saved to the SD card
int imageTransmissionRetries = 0;
int imageNumber = 0; // This is the id of the current image we are capturing. Used for storing the name of the image file into the above array

void setup() {
  initialize();       //Start everything with nominal values              (In Progress)
  //detectLaunch(24);   //Read MMA for launch acceleration signature        (DONE)
  //delay(40000);       //Wait to clear separation debris, in milliseconds  (DONE)  
  //detectGround(200);  //Read UTS for ground approach signature            (DONE)
  releaseParachute();       //Move parachute release servo                  (DONE)
  //delay(10000);       //Wait to land and settle, in milliseconds          (DONE)
  releaseTank();      //Move tank release servo                             (DONE)
  //driveTank();        //Move tank, drop marker, move tank                 (DONE)
  captureImages();    //Store 4 images to SD card                         (In Progress, hardware problem)
  transmitImages();   //Transfer 4 images via xBee to ground station      (In Progress)
}

void initialize() {
  parachute_servo.attach(9);      //Use pin 9
  parachute_servo.write(45);    

  tankRelease_servo.attach(10);   //Use pin 10
  tankRelease_servo.write(100);   

  mma = Adafruit_MMA8451();    //Initialize ACCELEROMETER and name it "MMA"
  mma.begin();                 //Begin comminucation with accelerometer
  delay(1000);                 // Needs time to initialize
  
  mma.setRange(MMA8451_RANGE_4_G);    //4_G for readings of accelerations of up to 4Gs

  pinMode(53, OUTPUT);         //Set the CS Pin as output
  SD.begin(53);                //Initialize the SD CARD READER using pin 53 for CS
  delay(1000); 								 // Time to initialize

  Serial.begin(115200);  //TESTING ONLY
  Serial1.begin(9600);    //Initialize xBee Serial Connection @ 9600 bps
}

void detectLaunch(float threshold) {            //Threshld for detection is in m/s^2
  int i = 0;
  bool detected = false;
  while(!detected) {
    sensors_event_t event;                      //Magical MMA Gobblety-Gook
    mma.getEvent(&event);                       //Magical MMA Gobblety-Gook
    if(event.acceleration.x > threshold) i++;   //Check if we are above the threshold for detection, if so, add to count
      else i = 0;                               //If the reading is not above the threshold, reest the counter.  (Thanks, Geyer)
    if(i >= 6) detected = true;                 //If the counter reaches 4, we have launched
    delay(100);                                 //Even Accelerometers needs a break sometimes
    }
  //if(detected == true) Serial1.println("DETECTED!");      //Commented out for now... Can add back in for debug...
}

void detectGround(float threshold) {         //Threshold for detection is in centimeters
  int i = 0;
  int distance = 300;                   //Temporary variable for distance information from the UTS (in cm)
  bool detected = false;
  while(!detected) {
    distance = sr04.Distance();         //Read in the distance from the UTS
    if(distance == 0) distance = 300;   //"Out of Range" returns "0", but we need "Out of Range" to report >200.  I choose 300.  -Bahn
    if(distance < threshold) i++;
      else i = 0;
    if(i >= 4) detected = true;
    delay(100);
    }
  //if(detected == true) Serial1.println("GROUND DETECTED!");  //Commented out for now... Can add back in for debug...
}

void releaseParachute() {
  parachute_servo.write(120);       //Move parachute release servo to unlocked position
  delay(500);
  parachute_servo.detach();
  delay(500);
}

void releaseTank() {
  tankRelease_servo.write(10);      //Move tank release servo to released position
  delay(500);
  tankRelease_servo.detach();
  delay(500);
}

void driveTank() {          //Three variables are DEFINED at the top of the program because they are some of the few things that WILL be changed on launch day
  //Set all of our pins to outputs    
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(17, OUTPUT);
  pinMode(16, OUTPUT);
  pinMode(15, OUTPUT);
  pinMode(14, OUTPUT);

  //Set everything to LOW
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
  digitalWrite(17, LOW);
  digitalWrite(16, LOW);
  digitalWrite(15, LOW);
  digitalWrite(14, LOW);

  delay(500);
  
  //Pair tank to controller
  digitalWrite(2, HIGH);
  delay(100);
  digitalWrite(17, HIGH);
  delay(100);
  digitalWrite(17, LOW);
  digitalWrite(2, LOW);
  delay(1000);
  
  //Drive forward
  digitalWrite(5, HIGH);
  digitalWrite(15, HIGH);
  delay(FORWARD_TIME);
  digitalWrite(5, LOW);
  digitalWrite(15, LOW);
  delay(500);
  
  //Turn tank right
  digitalWrite(15, HIGH);
  digitalWrite(3, HIGH);
  delay(SPIN_TIME);
  digitalWrite(15, LOW);
  digitalWrite(3, LOW);
  delay(500);
  
  //Turn turret left
  digitalWrite(14, HIGH);
  delay(TURRET_TIME);
  digitalWrite(14, LOW);
  delay(500);
  
  //Drive forward again
  digitalWrite(5, HIGH);
  digitalWrite(15, HIGH);
  delay(FORWARD_TIME);
  digitalWrite(5, LOW);
  digitalWrite(15, LOW);
  delay(300);
}  

void captureAndSaveImage(Adafruit_VC0706 camera) {
  //check that the card is present
  if (!SD.begin(53)) {
    return;
  }
  
  camera.begin();         //Initializes camera, ran once for each camera
  camera.setImageSize(VC0706_640x480);
  delay(3000);            //Needs time to initialize

  camera.takePicture();   //Takes picture with camera
  
  //Create an image with the name IMAGExx.JPG
  char filename[13];
  strcpy(filename, "IMAGE00.JPG");
  for (int i = 0; i < 100; i++) {
    filename[5] = '0' + i/10;
    filename[6] = '0' + i%10;
    //Create if does not exist, do not open existing, write, sync after write
    if (! SD.exists(filename)) {
      break;
    }
  }
  
  // Open the file for writing
  File imgFile = SD.open(filename, FILE_WRITE);

  // Get the size of the image (frame) taken  
  uint16_t jpglen = camera.frameLength();

  // Read all the data up to # bytes!
  while (jpglen > 0) {
    uint8_t *buffer;
    uint8_t bytesToRead = min(64, jpglen);
    buffer = camera.readPicture(bytesToRead);
    imgFile.write(buffer, bytesToRead);
    jpglen -= bytesToRead;
  }
  imgFile.close();
  imageFileNames[imageNumber] = filename;
  imageNumber++;
}

void captureImages(){
  // Initialize each camera and take a picture
  SoftwareSerial cam1SerialConnection(63, 62);   //Initialize Camera 1 connection
  Adafruit_VC0706 cam1 = Adafruit_VC0706(&cam1SerialConnection);  //start connection cam1
  captureAndSaveImage(cam1);
  cam1SerialConnection.end();

  SoftwareSerial cam2SerialConnection(65, 64);   //Initialize Camera 2 connection
  Adafruit_VC0706 cam2 = Adafruit_VC0706(&cam2SerialConnection);  //start connection cam2
  captureAndSaveImage(cam2);
  cam2SerialConnection.end();

  SoftwareSerial cam3SerialConnection(67, 66);   //Initialize Camera 3 connection
  Adafruit_VC0706 cam3 = Adafruit_VC0706(&cam3SerialConnection);  //start connection cam3
  captureAndSaveImage(cam3);
  cam3SerialConnection.end();

  SoftwareSerial cam4SerialConnection(68, 69);   //Initialize Camera 4 connection
  Adafruit_VC0706 cam4 = Adafruit_VC0706(&cam4SerialConnection);  //start connection cam4
  captureAndSaveImage(cam4);
  cam4SerialConnection.end();
}

void serialClear() {
  while(Serial1.available() > 0) {
    char k = Serial1.read();
  }
}

void transmitPicture(String filename) {
  serialClear();
  // Open the file for reading
  File imgFile = SD.open(filename, FILE_READ);
  unsigned int imgSize = imgFile.size();
  boolean While = 1;
  
  if(imgSize > 0) {     
    byte b[1];
    byte packet[100];
    for(unsigned int s = 0; s < (imgSize/100); s++) { //divide file into 1K sized chunks
      for(int i = 0; i < 100; i++) {                  //send 100 bytes
        imgFile.read(b, 1);
        Serial1.write(b[0]);
        packet[i] = b[0];
        delay(8);
      }
      
      Serial.print("Sent packet ");
      Serial.print(s);
      Serial.print(" of ");
      Serial.print((imgSize/100) + 1);
      Serial.print(", success: ");
      
      delay(50);                                   //after packet is sent, tell other xbee to check
      while(While == 1) {
        if(Serial1.read() != 49 && Serial1.read() != -1) {                   //check that no error is coming back
          //resend packet(" packets left in file ");
          Serial.println("ERROR: failed to send, retrying");
          serialClear();
          for(int m = 0; m < 100; m++) {                  //resend 100 bytes
            Serial1.write(packet[m]);
            delay(8);
          }
        }
        else if(Serial1.read() == 49) {
          Serial.println("check");
          serialClear();
          break;
        }
      }
    }
    Serial.println("Sending last packet...");
    for(unsigned int f = 0; f < imgSize%100; f++) {   //send whatever is left from the 100 byte chunks
      imgFile.read(b, 1);
      Serial1.write(b[0]);
      delay(8);
    }
    Serial1.flush();
  }
}

void transmitImages(){
  Serial.println("Sending image 1...");
  transmitPicture(imageFileNames[0]);
  delay(500);
  Serial.println("Sending image 2...");
  transmitPicture(imageFileNames[1]);
  delay(500);
  Serial.println("Sending image 3...");
  transmitPicture(imageFileNames[2]);
  delay(500);
  Serial.println("Sending image 4..");
  transmitPicture(imageFileNames[3]);
  delay(500);
}

void loop() {}    //leave empty, needed to compile
