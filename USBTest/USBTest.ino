
//This program is made to test the USB debugging functionality of the Mars Lander.
//It should print "Test: #" repeatedly with a DELAY millisecond long pause between each print out.
//The BAUD_RATE should match the baud rate of the serial monitor

#define BAUD_RATE 9600
#define DELAY 1000

int count = 0;

void setup() {
  Serial.begin(BAUD_RATE);
  count = 0;
}

void loop() {
  Serial.print("Test: ");
  Serial.println(count);
  count++;
  delay(DELAY);
}
