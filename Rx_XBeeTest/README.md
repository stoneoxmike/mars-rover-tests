# XBee Receiving (Rx) Test
The purpose of this program is to test the reception (and transmission) functionality of the XBee on the Mars Lander and Ground Station.  
When the program starts, the connection from the Mars Lander XBee and the Mars Lander Arduino MEGA will be initialized with the set ````BAUD_RATE````  
The program will then send a message saying "LOGRX TEST BEGIN|" through the XBee serial connection and should be received by the Ground Station.  
**If the Ground Station does not receive this initial message, either Rx on the Ground Station or Tx on the Mars Lander is not working.**  
Then the program will enter the PING Test Mode (see below).

## PING Test Mode
During the PING test mode, the program will wait to receive bytes from the Mars Lander XBee.  
If the bytes it receives are in the following sequence: ````PING#|````, then it will reply (transmit back) the following sequence: ````PONG#|````  
**The ````#```` in the PONG is the same from the PING.**

During this test, if bytes are received from the Mars Lander XBee, but the bytes do not form a complete command, the Ground Station will have ````TIMEOUT```` milliseconds to transmit the remaining bytes.  
As soon as another byte is read from the Mars Lander XBee, the "timeout timer" is reset, and the Ground Station has another ````TIMEOUT```` milliseconds to transmit more bytes.  
During this time, if a complete command if formed (see Commands below), the command will be processed and the timeout timer will be stopped until more bytes are received, which then the entire process starts over.  
However, if the Mars Lander XBee and MEGA do not receive any additional bytes from the Ground Station within ````TIMEOUT```` milliseconds, the current data that has been read from the XBee will be deleted/dumped/cleared, and the timeout timer will stop, and a message saying "TIMEDOUT" will be send via the Mars Lander XBee.  
**"TIMEDOUT" messages are indicators of bad connectivity.**

## Commands
The Mars Lander and Ground Station adhere to the following format for commands:
- An arbitrary length of characters that are then terminated by the pipe "|" character.
- For example, there could be a "FOOBAR" command that would look like: ````FOOBAR|````
- **The pipe character is known as the "command delimeter"**
- **The pipe character is what marks the end of one command. Any characters (or bytes) following the pipe character either belong to a new command or are raw data values (such as image files).**

## Baud Rate
To change the baud rate of the XBee connection, change the ````BAUD_RATE```` variable to the correct bits per second value (bps).

## Dialing in Baud Rate
To maximize speed (especially for image transmission), the baud rate for the XBee serial connection can be increased.  
However, increasing the speed too much may lead to more transmission errors, so it is a good idea to test an increased baud rate first.  
Possible baud rate values are: 1200, 2400, 4800, 9600, 19200, 38400, 57600, and 115200.  
  
To test a new baud rate:  
- Enter in the new baud rate in the ````BAUD_RATE```` variable.
- Upload the program to the Arduino MEGA
- Once uploaded, power off/unplug the MEGA
- Check the electrical wiring of the MEGA to the XBee (correct pins, wires seated correctly)
- Check the electical wiring of the Ground Station (correct pins, switch is one, wires seated correctly)
- Make sure your Ground Station has the correct baud rate set.
- Start up the Ground Station program (just have it up and listening).
- Power up the Arduino MEGA
- Check to see if the "LOGRX TEST BEGIN" message has been received on the Ground Station
- If it has been received, proceed. If it has not been received, double check your electrical wirings, repeat the last couple of steps, or try a lower baud rate.
- Issue a ping command, either through the Ground Station's buttons/functions, or by manually typing "PING0|" into the Serial Monitor.
- Watch to see if a corresponding "PONG0|" is received by the Ground Station.
- If one is received, try 5 more PING commands. If one is not received, begin troubleshooting
- If 5 PING commands are successful, try increasing the distance between the Ground Station and the Mars Lander and repeating the 5 PING test.
- If continue sending PING commands and increasing the distance until the estimated distance is reached and rhe PINGs are successful, or until the PINGs fail.  
If you can get consistent PONGs back from the Mars Lander at a considerable distance, then the baud rate you are using should work.
