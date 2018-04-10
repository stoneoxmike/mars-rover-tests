#include <Adafruit_MMA8451.h>

//Accelerometer Sensor Test
//Designed to test the functionality of the MMA accelerometer on the Mars Lander in detecting launch
//The program will send messages via xbee or USB (see config).
//The program can send a message when the correct amount of acceleration is detected (DETECTION_TEST mode), or it can
//send continuous acceleration data (RAW_TEST mode, see config), or it can take SAMPLES readings and report back the number of readings that met the threshold (BATCH_MODE).
//The program can take readings until it detects a specified (IN_A_ROW) number of consecutive readings that are above the given THRESHOLD value, which it will then send a message saying "DETECTED!"
//The program will wait STARTUP_DELAY milliseconds before beginning the test. STARTUP_DELAY should not be 0. Have it at least be 1000 milliseconds.
//The THRESHOLD is in meters per second squared.
//The SAMPLES_DELAY is in milliseconds, it is the amount of time that will be delayed before the next UTS reading is taken

//CONFIG
//If you would like the program to just detect the acceleration threshold, set the next value to true, otherwise set to false
#define DETECTION_TEST false

//If you would like the program to take SAMPLES number of readings from the sensor and report back how many were at or above the THRESHOLD, set the next value to true, otherwise set to false.
#define BATCH_TEST false

//If you would like the program to take readings until it detects IN_A_ROW consecutive readings that are above the specified THRESHOLD and will then message "DETECTED!"
#define BATCH_DETECTION_TEST false
#define IN_A_ROW 4

//If you would like the program to just send back a value stream of the acceleration readings, set the next value to true, otherwise false.
#define RAW_TEST true
//If RAW_TEST is set to true, the program will read straight from the sensor and report the raw value.
//To have the sensor take an average of SAMPLES readings before reporting a value, set the next value to true, Otherwise false.
#define AVERAGE_RAW true

//If you would like the program to report back via Xbee, change the next value from false to true
#define USE_XBEE false
//If you would like the program to report back via USB, keep the value above at false

#define THRESHOLD 11
#define SAMPLES 4
#define SAMPLES_DELAY 2

//Below is the number of milliseconds that the program will wait for before testing for another threshold detection after one is detected.
#define RESET_DELAY 1000

//The program will wait STARTUP_DELAY milliseconds before beginning the test.
#define STARTUP_DELAY 3000

#define BAUD_RATE 9600

Adafruit_MMA8451 mma;

typedef unsigned short USHORT;

#if USE_XBEE
void initSerial() {
  Serial2.begin(BAUD_RATE);
}

void message(String s) {
  Serial2.print("LOG" + s + "|");
}
#else
void initSerial() {
  Serial.begin(BAUD_RATE);
}

void message(String s) {
  Serial.println(s);
}
#endif

double getXAcceleration() {
  sensors_event_t event; 
  mma.read();
  mma.getEvent(&event);
  return event.acceleration.x;
}

double getAverageXAcceleration(int sampleCount) {
  double total = 0;
  sensors_event_t event; 
  for(USHORT i = 0; i < sampleCount; i++) {
    mma.read();
    mma.getEvent(&event);
    total += event.acceleration.x;
    delay(SAMPLES_DELAY);
  }
  return total/((double) sampleCount);
}

void wait_for_acceleration(double threshold) {
  double x = 0;
  bool detected = false;
  while(!detected) {
    x = getAverageXAcceleration(SAMPLES);
    detected = threshold <= x;
  }
}

void setup() {
  initSerial(); // Set up the serial connection
  message("TEST START"); // Send message showing start of program
  mma = Adafruit_MMA8451(); // Init the accelerometer
  message("MMA FOUND"); // Reply that the accelerometer has been found
  mma.setRange(MMA8451_RANGE_2_G); // Set the range of the accelerometer
  delay(STARTUP_DELAY); // Wait for STARTUP_DELAY milliseconds. Note this is important! The MMA needs time to calibrate/boot up
}

void loop() {
  if(DETECTION_TEST) {
    wait_for_acceleration(THRESHOLD);
    message("DETECTED");
    delay(RESET_DELAY);
  }
  else if(RAW_TEST) {
    if(AVERAGE_RAW) {
      String v = String(getAverageXAcceleration(SAMPLES));
      message(v);
    }
    else {
      String v = String(getXAcceleration());
      message(v);
      delay(SAMPLES_DELAY);
    }
  }
  else if(BATCH_TEST) {
    int count = 0;
    for(int i = 0; i < SAMPLES; i++) {
      double x = getXAcceleration();
      if(x >= THRESHOLD) {
        count++; // One of the readings is over or equal to the threshold, increase the count
      }
    }
    message(String(count));
  }
  else if(BATCH_DETECTION_TEST) {
    int count = 0;
    bool detected = false;
    while(!detected) {
	it(getXAcceleration() >= THRESHOLD) {
	  count++;
	}
	else {
	  count = 0;
	}

	detected = (count >= IN_A_ROW);
	delay(SAMPLES_DELAY);
    }
    if(detected) {
	message("DETECTED!");
    }
  }
}
