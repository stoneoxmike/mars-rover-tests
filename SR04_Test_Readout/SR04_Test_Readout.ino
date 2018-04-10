//UTS Test Program - Testing the ultrasonic sensor on the Mars Lander
//The program will begin the serial connection with a "HELLO" message, and then proceed
//to take readings from the ultrasonic sensor every SAMPLES_DELAY milliseconds and then report
//back the number of centimeters that it reads.
//At the same time it will run the running average function, and will report back the running average and if the threshold was reached.
//
// The format of the USB report is: [distance cm], [running average distance], [detected (yes/no)]
//
// THRESHOLD is in centimeters
// SAMPLES_DELAY is in milliseconds
#include "SR04.h"

#define UTS_SAMPLES 4
#define SAMPLES_DELAY 2
#define THRESHOLD 200

#define TRIG_PIN 7
#define ECHO_PIN 6

#define BAUD_RATE 9600

#define s Serial

typedef unsigned short USHORT;

SR04 sr04 = SR04(ECHO_PIN,TRIG_PIN);

int readingIter = 0;
int average = 0;
int readings[UTS_SAMPLES];
int totalCM = 5000;
bool detected = false;

long read_uts_sensor() {
  return sr04.Distance();
}

void setup() {
   s.begin(BAUD_RATE);
   s.println("HELLO");
   for(USHORT i = 0; i < UTS_SAMPLES; i++){
    readings[i] = 500;
  }
}

void loop() {
    totalCM = totalCM - readings[readingIter];
    readings[readingIter] = read_uts_sensor();
    totalCM = totalCM + readings[readingIter];
    average = totalCM/UTS_SAMPLES;
    detected = THRESHOLD >= average;
    s.print("LOG");
    s.print(readings[readingIter]);
    s.print(",");
    s.print(average);
    s.print(",");
    s.println((detected ? "yes" : "no"));
    delay(SAMPLES_DELAY);
    readingIter += 1;
    if(readingIter >= UTS_SAMPLES) {
      readingIter = 0;
    }
}
