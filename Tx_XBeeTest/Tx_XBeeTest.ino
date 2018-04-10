//This program is made to test the XBee transmission functionality of the Mars Lander.
//It should print "Test: #" repeatedly with a DELAY millisecond long pause between each print out.
//The BAUD_RATE should match the baud rate of the serial/xbee monitor

#define BAUD_RATE 9600
#define DELAY 1000

#define xbee Serial1

int count = 0;

void setup() {
  xbee.begin(BAUD_RATE);
  count = 0;
}

void loop() {
  xbee.print("LOGTest: ");
  xbee.print(count);
  xbee.print("|");
  count++;
  delay(DELAY);
}
