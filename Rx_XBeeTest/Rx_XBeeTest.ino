//This program is made to test the XBee receiving functionality of the Mars Lander.
//The program will wait and listen for a message from the XBee in the following format:
//PING#|
//The program will then reply with a message of: "PONG#|" the number that was in the "PING" will be returned in the "PONG"
//The BAUD_RATE should match the baud rate of the serial/xbee monitor

#define BAUD_RATE 9600

// This is the number of milliseconds that one byte has to follow another byte when sending a full command.
#define TIMEOUT 2000

#define xbee Serial1

String incomingBuffer = "";
long last_message = millis(); // This is the timestamp when the last byte was received
bool waiting = false; // This variable tells whether or not we are waiting for a full command to finish being sent.

long count = 0;

void message(String text) {
  xbee.print("LOG"+text+"|");
  xbee.flush();
}

void setup() {
  xbee.begin(BAUD_RATE);
  message("RX TEST BEGIN");
}

void doPing(String tag) {
  xbee.print("PONG" + tag.substring(4,tag.length()) + "|");
  xbee.flush();
}

void processPacket(String packet) { // Check to see if what we received is a PING command
  String tag = packet.substring(0, packet.length()-1); // This strips off the delimiter (the "|" character) and leaves just the command
  if(tag.substring(0,4) == "PING") {
    doPing(tag);
  }
}

void loop() {
  if(xbee.available() > 0) {
    while(xbee.available() > 0) {
        waiting = true;
        char c = xbee.read(); // Read the byte from the XBee
        incomingBuffer += c; // Add the received byte to the command buffer
        last_message = millis(); // Update the timestamp of the last received byte.
        if(c=='|') {
          processPacket(incomingBuffer);
          waiting = false;
          incomingBuffer = "";
        }
      }
  }
  else {
    if(waiting && ((millis() - last_message) > TIMEOUT)) {
      message("TIMEDOUT");
      waiting = false;
      incomingBuffer = "";
    }
  }
}
