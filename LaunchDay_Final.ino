//Final Version of Program for Launch Day
//DO NOT CHANGE UNLESS APPROVED BY BAHN!

#include <Servo.h>
#include <SD.h>
#include <SPI.h>
#include <MMA8451.h>
#include <Adafruit_VC0706.h>
#include <SoftwareSerial.h>
#include <SR04.h>

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

void loop() {}  //needed to compile
