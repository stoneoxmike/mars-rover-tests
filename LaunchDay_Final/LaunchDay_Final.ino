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

void setup() {
  initialize();       //Turn everything on                                (In Progress)  
  detectLaunch(15);   //Read MMA for launch acceleration signature        (DONE, needs final testing)  Note: We should try changing the range to mma.setRange(MMA8451_RANGE_4_G); and readig some data.  Then we can detect a larger acceleration and rule out more noise. Or, make it 6 - 8 consecutive readings.
  delay(40000);       //Wait to clear separation debris, in milliseconds  (DONE, agree on number)  Sensor read "Out of Range" from T+20 seconds to T+105 seconds during February launch.
  detectGround(200);  //Read UTS for ground approach signature            (DONE, based on detectLaunch, untested)  (Approach data from Feb: 77, 126, 138, out, out, out, out, out, 312, 275, 246, 220, 191, 160, 133, 110, 91, 67, 48, 16, 10)
  releaseParachute(); //Move parachute release servo                      (Needs numbers specified)
  delay(10000);       //Wait to land and settle, in milliseconds          (DONE)
  releaseTank();      //Move tank release servo                           (DONE, needs testing)
  driveTank();        //Move tank, drop marker, move tank                 (DONE, needs testing)
  captureImages();    //Store 4 images to SD card                         (In Progress)
  transmitIamges();   //Transfer 4 images via xBee to ground station      (In Progress)
}


void initialize(){
  Servo parachute_servo;        //Initialize PARACHUTE Servo
  parachute_servo.attach(9);    //Use pin 9
  parachute_servo.write(45);       //NEEDS A SPECIFIC NUMBER
  
  Servo tankRelease_servo;      //Initialize TANK Release Servo
  tankRelease_servo.attach(10);   //Use pin 10
  tankRelease_servo.write(100);
  
  Adafruit_MMA8451 mma = Adafruit_MMA8451();    //Initialize ACCELEROMETER and name it "MMA"
  mma.begin();                                   //Begin comminucation with accelerometer
  mma.setRange(MMA8451_RANGE_4_G);              //NOTE: This had been set to 2_G, I recommend changing to 4_G and setting the detection threshold to >20 m/s^2.  There are instances in the data from February that would have caused a pre-mature detection.
  
  SR04 sr04 = SR04(6, 7);          //Initialize UTS and name it "sr04"  (Echo_pin = 6, Trig_pin = 7)
  
  SD.begin(53);                   //Initialize the SD CARD READER using pin 53 for CS
  
  SoftwareSerial cam1SerialConnection = SoftwareSerial(63, 62);   //Initialize Camera 1
  Adafruit_VC0706 cam1 = Adafruit_VC0706(&cam1SerialConnection);  //and name it "cam1"
  
  SoftwareSerial cam2SerialConnection = SoftwareSerial(65, 64);   //Initialize Camera 2
  Adafruit_VC0706 cam2 = Adafruit_VC0706(&cam2SerialConnection);  //and name it "cam2"
  
  SoftwareSerial cam3SerialConnection = SoftwareSerial(67, 66);   //Initialize Camera 3
  Adafruit_VC0706 cam3 = Adafruit_VC0706(&cam3SerialConnection);  //and name it "cam3"

  SoftwareSerial cam4SerialConnection = SoftwareSerial(69, 68);   //Initialize Camera 4
  Adafruit_VC0706 cam4 = Adafruit_VC0706(&cam4SerialConnection);  //and name it "cam4"
  
  Serial1.begin(9600);    //Initialize xBee Serial Connection @ 9600 bps
}

void detectLaunch(float threshold){             //Threshld for detection is in m/s^2
  int i = 0;
  bool detected = false;
  while(!detected) {
    sensors_event_t event;                      //Magical MMA Gobblety-Gook
    mma.getEvent(&event);                       //Magical MMA Gobblety-Gook
    if(event.acceleration.x > threshold) i++;   //Check if we are above the threshold for detection, if so, add to count
      else i = 0;                               //If the reading is not above the threshold, reest the counter.  (Thanks, Geyer)
    if(i >= 4) detected = true;                 //If the counter reaches 4, we have launched
    delay(100);                                 //Even Accelerometers needs a break sometimes
    }
  //if(detected == true) Serial1.println("DETECTED!");      //Commented out for now... Can add back in for debug...
}

void detectGround(float threshold){         //Threshold for detection is in cm.
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

void releaseParachute(){
  parachute_servo.write(120);
}

void releaseTank(){
  tankRelease_servo.write(10);
}

void driveTank(){          //Three variables are DEFINED at the top of the program because they are some of the few things that WILL be changed on launch day
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

void captureImages(){
  
}

void transmitImages(){
  
}

void loop() {}    //leave empty, needed to compile
