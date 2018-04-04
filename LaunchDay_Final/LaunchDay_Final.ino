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
#define SPIN_TIME 500
#define TURRET_TIME 3000

Servo parachute_servo;        //Initialize PARACHUTE Servo
Servo tankRelease_servo;      //Initialize TANK Release Servo

Adafruit_MMA8451 mma;         //Create variable mma to be referenced later by other functions

SR04 sr04  = SR04(6, 7);      //Initialize UTS and name it "sr04"  (Echo_pin = 6, Trig_pin = 7)
  
SoftwareSerial cam1SerialConnection = SoftwareSerial(63, 62);   //Initialize Camera 1
Adafruit_VC0706 cam1 = Adafruit_VC0706(&cam1SerialConnection);  //and name it "cam1"
  
SoftwareSerial cam2SerialConnection = SoftwareSerial(65, 64);   //Initialize Camera 2
Adafruit_VC0706 cam2 = Adafruit_VC0706(&cam2SerialConnection);  //and name it "cam2"
  
SoftwareSerial cam3SerialConnection = SoftwareSerial(67, 66);   //Initialize Camera 3
Adafruit_VC0706 cam3 = Adafruit_VC0706(&cam3SerialConnection);  //and name it "cam3"

SoftwareSerial cam4SerialConnection = SoftwareSerial(69, 68);   //Initialize Camera 4
Adafruit_VC0706 cam4 = Adafruit_VC0706(&cam4SerialConnection);  //and name it "cam4"
  
void setup() {
  initialize();       //Start everything with nominal values              (In Progress)
  detectLaunch(24);   //Read MMA for launch acceleration signature        (DONE)
  delay(40000);       //Wait to clear separation debris, in milliseconds  (DONE)  
  detectGround(200);  //Read UTS for ground approach signature            (DONE, based on detectLaunch, untested)  (Approach data from Feb: 77, 126, 138, out, out, out, out, out, 312, 275, 246, 220, 191, 160, 133, 110, 91, 67, 48, 16, 10)
  parachute_servo.write(120); //Move parachute release servo              (DONE, needs testing)
  delay(10000);       //Wait to land and settle, in milliseconds          (DONE)
  tankRelease_servo.write(10);      //Move tank release servo             (DONE, needs testing)
  driveTank();        //Move tank, drop marker, move tank                 (DONE, needs testing)
  captureImages();    //Store 4 images to SD card                         (In Progress)
  transmitImages();   //Transfer 4 images via xBee to ground station      (In Progress)
}

void initialize() {
  parachute_servo.attach(9);    //Use pin 9
  parachute_servo.write(45);    

  tankRelease_servo.attach(10);   //Use pin 10
  tankRelease_servo.write(100);   

  mma = Adafruit_MMA8451();    //Initialize ACCELEROMETER and name it "MMA"
  mma.begin();                        //Begin comminucation with accelerometer
  delay(1000); // Needs time to initialize
  
  mma.setRange(MMA8451_RANGE_4_G);    //NOTE: This had been set to 2_G, I recommend changing to 4_G and setting the detection threshold to >20 m/s^2.  There are instances in the data from February that would have caused a pre-mature detection.

  pinMode(53, OUTPUT);        //Set the CS Pin as output
  SD.begin(53);               //Initialize the SD CARD READER using pin 53 for CS
  delay(1000); // Time to initialize
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

void detectGround(float threshold) {         //Threshold for detection is in cm.
  int i = 0;
  bool detected = false;
  while(!detected) {
    if(sr04.Distance() < threshold) i++;    //I think that when this is "Out of Range" it reports "0".  If so, we need top address that.  We'll check it tomorrow in class.
      else i = 0;
    if(i >= 4) detected = true;
    delay(100);
    }
  //if(detected == true) Serial1.println("GROUND DETECTED!");  //Commented out for now... Can add back in for debug...
}

void driveTank() {          //Three variables are DEFINED at the top of the program because they are some of the few things that WILL be changed on launch day
  //Set all of our pins to outputs    
  pinMode(16, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(17, OUTPUT);
  pinMode(14, OUTPUT);
  pinMode(15, OUTPUT);

  //Set everything to LOW
  digitalWrite(16, LOW);
  digitalWrite(5, LOW);
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(17, LOW);
  digitalWrite(14, LOW);
  digitalWrite(15, LOW);
  
  //Pair tank to controller
  digitalWrite(15, HIGH);
  delay(100);
  digitalWrite(14, HIGH);
  delay(100);
  digitalWrite(14, LOW);
  digitalWrite(15, LOW);
  delay(3000);
  
  //Drive forward
  digitalWrite(16, HIGH);
  digitalWrite(5, HIGH);
  delay(FORWARD_TIME);
  digitalWrite(16, LOW);
  digitalWrite(5, LOW);
  delay(300);
  
  //Turn tank right
  digitalWrite(16, HIGH);
  digitalWrite(3, HIGH);
  delay(SPIN_TIME);
  digitalWrite(16, LOW);
  digitalWrite(3, LOW);
  delay(300);
  
  //Turn turret right
  digitalWrite(17, HIGH);
  delay(TURRET_TIME);
  digitalWrite(17, LOW);
  delay(300);
  
  //Drive forward again
  digitalWrite(16, HIGH);
  digitalWrite(5, HIGH);
  delay(FORWARD_TIME);
  digitalWrite(16, LOW);
  digitalWrite(5, LOW);
  delay(300);
}  

void initializeCamera(Adafruit_VC0706 camera) {
  camera.begin(); 
  delay(1000); // Needs time to initialize
  camera.setImageSize(VC0706_640x480);  
}

void captureAndSaveImage(Adafruit_VC0706 camera) {
  // Create an image with the name IMAGExx.JPG
  char filename[13];
  strcpy(filename, "IMAGE00.JPG");
  for (int i = 0; i < 100; i++) {
    filename[5] = '0' + i/10;
    filename[6] = '0' + i%10;
    // create if does not exist, do not open existing, write, sync after write
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
    uint8_t bytesToRead = min(32, jpglen);
    buffer = camera.readPicture(bytesToRead);
    imgFile.write(buffer, bytesToRead);
    jpglen -= bytesToRead;
  }
  imgFile.close();
}

void captureImages(){
  // Initialize each camera and take a picture
  initializeCamera(cam1);
  captureAndSaveImage(cam1);
  
  initializeCamera(cam2);
  captureAndSaveImage(cam2);
  
  initializeCamera(cam3);
  captureAndSaveImage(cam3);
  
  initializeCamera(cam4);
  captureAndSaveImage(cam4);
}

void transmitImages(){
  
}

void loop() {}    //leave empty, needed to compile
