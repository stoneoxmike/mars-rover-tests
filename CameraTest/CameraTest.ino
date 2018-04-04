#include <Adafruit_VC0706.h>
#include <SPI.h>
#include <SD.h>

#define CAM_1_ENABLED false
#define CAM_2_ENABLED false
#define CAM_3_ENABLED false
#define CAM_4_ENABLED false
#define ALL_CAMS_ENABLED false

//TTL Camera Pins
#define CAMERA_1_TX_PIN 63 //Connect this pin to "TX" of the TTL camera (A9)
#define CAMERA_1_RX_PIN 62 //Connect this pin to "RX" of the TTL camera (A8)
#define CAMERA_2_TX_PIN 65 // (A10)
#define CAMERA_2_RX_PIN 64 // (A11)
#define CAMERA_3_TX_PIN 67 // (A12)
#define CAMERA_3_RX_PIN 66 // (A13)
#define CAMERA_4_TX_PIN 69 // (A14)
#define CAMERA_4_RX_PIN 68 // (A15)

#define BAUD_RATE 9600

// This is the pin number that is connected to the "CS" pin on the SD card shield. Don't change this.
#define chipSelect 53

void captureAndSaveImage(Adafruit_VC0706 camera) {
  Serial.println("Snap in 3 secs...");
  delay(3000);

  if (!camera.takePicture()) {
    Serial.println("Failed to snap!");
  }
  else {
    Serial.println("Picture taken!");
  }
  
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
  Serial.print("Storing ");
  Serial.print(jpglen, DEC);
  Serial.print(" byte image.");

  int32_t time = millis();
  pinMode(8, OUTPUT);
  // Read all the data up to # bytes!
  while (jpglen > 0) {
    uint8_t *buffer;
    uint8_t bytesToRead = min(32, jpglen);
    buffer = camera.readPicture(bytesToRead);
    imgFile.write(buffer, bytesToRead);
    jpglen -= bytesToRead;
  }
  imgFile.close();

  time = millis() - time;
  Serial.println("done!");
  Serial.print(time); Serial.println(" ms elapsed");
}

void initializeCamera(Adafruit_VC0706 camera) {
  // Try to locate the camera
  if (camera.begin()) {
    Serial.println("Camera Found:");
  } else {
    Serial.println("No camera found?");
    return;
  }
  
  // Print out the camera version information (optional)
  char *reply = camera.getVersion();
  if (reply == 0) {
    Serial.print("Failed to get version");
  } else {
    Serial.println("-----------------");
    Serial.print(reply);
    Serial.println("-----------------");
  }

  
  camera.setImageSize(VC0706_640x480);        // biggest
  Serial.println("Finished initializing camera.");
}

void setup() {
  pinMode(53, OUTPUT); // Initialize the slave select pin for SPI
  
  Serial.begin(BAUD_RATE);
  Serial.println("JPEG Camera Test.");
  
  // see if the SD card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Error: Card failed, or not present");
    return;
  }
  else {
    Serial.println("SD Card found...");  
  }

  // Test each camera that is enabled
  if(CAM_1_ENABLED || ALL_CAMS_ENABLED) {
    SoftwareSerial cam1Connection = SoftwareSerial(CAMERA_1_TX_PIN, CAMERA_1_RX_PIN);
    Adafruit_VC0706 cam1 = Adafruit_VC0706(&cam1Connection);
    initializeCamera(cam1);
    captureAndSaveImage(cam1);
  }
  if(CAM_2_ENABLED || ALL_CAMS_ENABLED) {
    SoftwareSerial cam2Connection = SoftwareSerial(CAMERA_2_TX_PIN, CAMERA_2_RX_PIN);
    Adafruit_VC0706 cam2 = Adafruit_VC0706(&cam2Connection);
    initializeCamera(cam2);
    captureAndSaveImage(cam2);
  }
  if(CAM_3_ENABLED || ALL_CAMS_ENABLED) {
    SoftwareSerial cam3Connection = SoftwareSerial(CAMERA_3_TX_PIN, CAMERA_3_RX_PIN);
    Adafruit_VC0706 cam3 = Adafruit_VC0706(&cam3Connection);
    initializeCamera(cam3);
    captureAndSaveImage(cam3);
  }
  if(CAM_4_ENABLED || ALL_CAMS_ENABLED) {
    SoftwareSerial cam4Connection = SoftwareSerial(CAMERA_4_TX_PIN, CAMERA_4_RX_PIN);
    Adafruit_VC0706 cam4 = Adafruit_VC0706(&cam4Connection);
    initializeCamera(cam4);
    captureAndSaveImage(cam4);
  }
}

void loop() { // Do nothing.
}

