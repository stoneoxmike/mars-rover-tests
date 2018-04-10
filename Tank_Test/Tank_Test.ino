#include <Servo.h>

//Tank Test Program
//This program is designed to test the pairing and piloting functionality of the Tank and Mars Lander.
//The program will delay for 2 seconds, pair with the tank, pause a second, drive the tank forward for FORWARD_TIME milliseconds
//and then stop the tank, turn the turrent to the left for TURRENT_TIME milliseconds, and the then the turrent right for TURRENT_TIME milliseconds.
//The tank will then be piloted to spin right for SPIN_TIME milliseconds, and then drive forward for FORWARD_TIME milliseconds.

//If you would like to also test the servo release of the tank (locking and releasing the tank), set the next value to true:
#define RELEASE_TEST false

#define FORWARD_TIME 5000
#define SPIN_TIME 500
#define TURRENT_TIME 3000

//The tank pins are provided below:
#define TANK_LF_PIN 16
#define TANK_RF_PIN 5
#define TANK_LB_PIN 2
#define TANK_RB_PIN 3
#define TANK_TL_PIN 4
#define TANK_TR_PIN 17
#define TANK_POWER_PIN 14
#define TANK_FUNC_PIN 15

#if RELEASE_TEST
//This is the pin number that the tank release servo is attached to
#define TANK_SERVO_PIN 9

//This is the degree that the tank release servo is set to in order to release the tank
#define TANK_RELEASE_DEGREES 10

//This is the degree that the tank release servo is set to in order to lock the tank in place
#define TANK_LOCKED_DEGREES 100

Servo tank_servo;

void release_tank() {
  tank_servo.write(TANK_RELEASE_DEGREES);
  delay(250);
}

void lock_tank() {
  tank_servo.write(TANK_LOCKED_DEGREES);
  delay(250);
}
#endif

void forward(int mil) {
  digitalWrite(TANK_LF_PIN, HIGH);
  digitalWrite(TANK_RF_PIN, HIGH);
  delay(mil);
  digitalWrite(TANK_LF_PIN, LOW);
  digitalWrite(TANK_RF_PIN, LOW);
  delay(300);
}

void backward(int mil) {
  digitalWrite(TANK_LB_PIN, HIGH);
  digitalWrite(TANK_RB_PIN, HIGH);
  delay(mil);
  digitalWrite(TANK_LB_PIN, LOW);
  digitalWrite(TANK_RB_PIN, LOW);
  delay(300);
}

void spin_right(int mil) {
  digitalWrite(TANK_LF_PIN, HIGH);
  digitalWrite(TANK_RB_PIN, HIGH);
  delay(mil);
  digitalWrite(TANK_LF_PIN, LOW);
  digitalWrite(TANK_RB_PIN, LOW);
  delay(300);
}

void spin_left(int mil) {
  digitalWrite(TANK_LB_PIN, HIGH);
  digitalWrite(TANK_RF_PIN, HIGH);
  delay(mil);
  digitalWrite(TANK_LB_PIN, LOW);
  digitalWrite(TANK_RF_PIN, LOW);
  delay(300);
}

void drop_marker() { //Turn the turrent right and then back to its position
  digitalWrite(TANK_TR_PIN, HIGH);
  delay(TURRENT_TIME);
  digitalWrite(TANK_TR_PIN, LOW);
  delay(100);
  digitalWrite(TANK_TL_PIN, HIGH);
  delay(TURRENT_TIME);
  digitalWrite(TANK_TL_PIN, LOW);
  delay(300);
}

void pair_tank() { // Based upon Micheals code that works
  digitalWrite(TANK_FUNC_PIN, HIGH);
  delay(100);
  digitalWrite(TANK_POWER_PIN, HIGH);
  delay(100);
  digitalWrite(TANK_POWER_PIN, LOW);
  digitalWrite(TANK_FUNC_PIN, LOW);
  delay(3000);
}

void init_pins() {
  //Set all of our pins to outputs
  pinMode(TANK_LF_PIN, OUTPUT);
  pinMode(TANK_RF_PIN, OUTPUT);
  pinMode(TANK_LB_PIN, OUTPUT);
  pinMode(TANK_RB_PIN, OUTPUT);
  pinMode(TANK_TL_PIN, OUTPUT);
  pinMode(TANK_TR_PIN, OUTPUT);
  pinMode(TANK_POWER_PIN, OUTPUT);
  pinMode(TANK_FUNC_PIN, OUTPUT);

  //Set everything to LOW
  digitalWrite(TANK_LF_PIN, LOW);
  digitalWrite(TANK_RF_PIN, LOW);
  digitalWrite(TANK_LB_PIN, LOW);
  digitalWrite(TANK_RB_PIN, LOW);
  digitalWrite(TANK_TL_PIN, LOW);
  digitalWrite(TANK_TR_PIN, LOW);
  digitalWrite(TANK_FUNC_PIN, LOW);
  digitalWrite(TANK_POWER_PIN, LOW);
  #if RELEASE_TEST // If we are testing the tank servo locking and releasing, initialize the servo motor.
    tank_servo.attach(TANK_SERVO_PIN);
  #endif
}

void setup() {
  init_pins(); // Set up all of the pins and possibly servo if it is enabled
  #if RELEASE_TEST  // Lock and release the tank if this feature is enabled
    lock_tank();
    delay(2000);
    release_tank();
    delay(500);
  #else
    delay(2000);
  #endif
  pair_tank(); // Pair with the tank
  delay(1000);
  forward(FORWARD_TIME); // Move forward
  delay(500);
  drop_marker(); // Spin the turrent to drop the marker
  delay(500);
  spin_right(SPIN_TIME); // Spin right
  delay(500);
  forward(FORWARD_TIME); // Move forward
  delay(500); // Finished
}

void loop() {
  //Do nothing.
}
