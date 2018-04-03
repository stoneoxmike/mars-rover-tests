//Final Version of Program for Launch Day
//DO NOT CHANGE UNLESS APPROVED BY BAHN!
//This is NOT for testing subsystems.  This is a one-shot, here-we-go, final version.

#include <Servo.h>
#include <SD.h>
#include <SPI.h>
#include <MMA8451.h>
#include <Adafruit_VC0706.h>
#include <SoftwareSerial.h>
#include <SR04.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

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
  
  Servo tankRelease_servo;      //Initialize Tank Release Servo
  tankRelease_servo.attach(10);   //WHAT PIN IS THIS???
  
  Adafruit_MMA8451 mma = Adafruit_MMA8451();    //Initialize accelerometer and name it "MMA"
  
  SR04 sr04 = SR04(6, 7);          //Initialize UTS and name it "sr04"  (Echo_pin = 6, Trig_pin = 7)
  
  SoftwareSerial cam1SerialConnection = SoftwareSerial(CAMERA_1_TX_PIN, CAMERA_1_RX_PIN);   //Initialize Camera 1
  Adafruit_VC0706 cam1 = Adafruit_VC0706(&cam1SerialConnection);                            //and name it "cam1"
  
  SoftwareSerial cam2SerialConnection = SoftwareSerial(CAMERA_2_TX_PIN, CAMERA_2_RX_PIN);   //Initialize Camera 2
  Adafruit_VC0706 cam2 = Adafruit_VC0706(&cam2SerialConnection);                            //and name it "cam2"
  
  SoftwareSerial cam3SerialConnection = SoftwareSerial(CAMERA_3_TX_PIN, CAMERA_3_RX_PIN);   //Initialize Camera 3
  Adafruit_VC0706 cam3 = Adafruit_VC0706(&cam3SerialConnection);                            //and name it "cam3"

  SoftwareSerial cam4SerialConnection = SoftwareSerial(CAMERA_4_TX_PIN, CAMERA_4_RX_PIN);   //Initialize Camera 4
  Adafruit_VC0706 cam4 = Adafruit_VC0706(&cam4SerialConnection);                            //and name it "cam4"
  
  Serial1.begin(9600);    //Initialize xBee Serial Connection @ 9600 bps
}

void detectLaunch(){

}

void detectGround(){

}

void releaseParachute(){
  
}

void releaseTank(){
  
}

void driveTank(){
  
}

void captureImages(){
  
}

void transmitImages(){
  
}

void loop() {}    //leave empty, needed to compile
