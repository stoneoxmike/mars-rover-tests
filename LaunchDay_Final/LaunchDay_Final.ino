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
  initialize();       //Turn everything on
  detectLaunch();     //Read MMA for launch acceleration signature
  delay(60000);       //Read UTS for ground approach signature
  releaseParachute(); //Move parachute release servo
  delay(10000);       //Wait to land and settle
  releaseTank();      //Move tank release servo
  driveTank();        //Move tank, drop marker, move tank
  captureImages();    //Store 4 images to SD card
  transmitIamges();   //Transfer 4 images via xBee to ground station
}

void initialize(){
  Servo parachute_servo;        //Initialize Parachute Servo
  parachute_servo.attach(9);    //Use pin 9
  parachute_servo.write(lockdegrees);
  
  Servo tankRelease_servo;      //Initialize Tank Release Servo
  tankRelease_servo.attach(10);   //Use pin 10
  tankRelease_servo.write(100);
  
  Adafruit_MMA8451 mma = Adafruit_MMA8451();    //Initialize accelerometer and name it "MMA"
  
  SR04 sr04 = SR04(6, 7);          //Initialize UTS and name it "sr04"  (Echo_pin = 6, Trig_pin = 7)
  
  SD.begin(53);                   //Initialize the SD card reader using pin 53 for CS
  
  SoftwareSerial cam1SerialConnection = SoftwareSerial(63, 62);   //Initialize Camera 1
  Adafruit_VC0706 cam1 = Adafruit_VC0706(&cam1SerialConnection);                            //and name it "cam1"
  
  SoftwareSerial cam2SerialConnection = SoftwareSerial(65, 64);   //Initialize Camera 2
  Adafruit_VC0706 cam2 = Adafruit_VC0706(&cam2SerialConnection);                            //and name it "cam2"
  
  SoftwareSerial cam3SerialConnection = SoftwareSerial(67, 66);   //Initialize Camera 3
  Adafruit_VC0706 cam3 = Adafruit_VC0706(&cam3SerialConnection);                            //and name it "cam3"

  SoftwareSerial cam4SerialConnection = SoftwareSerial(69, 68);   //Initialize Camera 4
  Adafruit_VC0706 cam4 = Adafruit_VC0706(&cam4SerialConnection);                            //and name it "cam4"
  
  Serial1.begin(9600);    //Initialize xBee Serial Connection @ 9600 bps
}

void detectLaunch(){

}

void detectGround(){

}

void releaseParachute(){
  parachute_servo.write(releasedegrees);
}

void releaseTank(){
  tankRelease_servo.write(10);
}

void driveTank(){                     //Michael, it seems that you cannont define a function within a function.  I suggest the following...
  //Set all of our pins to outputs    //I put the DEFINES at the top because the are one of the few thigns that WILL be changed that day.
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
