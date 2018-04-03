FILE IS OUT OF DATE

# Accelerometer Test
This test is designed to test the *Adafruit MMA8451* accelerometer mounted on the Mars Lander.  
Currently, the test is designed to run on the **positive X-Axis**, but the code may be modified to test in other axies.  
**At the start of the program the following will occur:**  
- USB or XBee connection will be started (at the baud rate set by the ````BAUD_RATE```` variable, **see Configure Serial**)
- A message saying "TEST START" will be set via the Serial connection (**see Configure Serial**)
- The MMA8451 accelerometer will be initialized
-- If the initialization is successful, a message saying "MMA FOUND" will be sent via the Serial connection.
- The program will pause for ````STARTUP_DELAY```` milliseconds (time to physically get set for the test if doing a "blast off" test).
- The program will then begin the test in the specified mode.

**There are three test modes:**  
- ````DETECTION````  
- ````BATCH```` 
- ````RAW````  

**Each mode is detailed below:**

## Detection Mode  
#### Config
Please set the ````DETECTION_TEST```` preprocessor variable to ````true```` and set the other  
test mode variables (BATCH_TEST and RAW_TEST) to ````false````.  
#### Method
In detection mode, the program will wait until the accelerometer reads an average X-acceleration that is greater than or equal to the threshold defined by the ````THRESHOLD```` variable.  
When the threshold is met, the string "DETECTED" will be printed via the XBee or the USB Serial connection (see Configure Serial).  
The average x-acceleration is found by taking a number of readings equal to the ````SAMPLES```` variable. There is a delay of ````SAMPLES_DELAY```` milliseconds between each sample is taken.  
It is a good idea to keep ````SAMPLES_DELAY```` low (1-100).
  
## Batch Mode
#### Config
Please set the ````BATCH_TEST```` preprocessor variable to ````true```` and set the other
test mode variables (DETECTION_TEST, BATCH_DETECTION_TEST and RAW_TEST) to ````false````.
#### Method
In the batch mode, the program will take a number x-acceleration readings and report back what number of samples were equal to or above the threshold value defined by the ````THRESHOLD```` variable.  
The number of x-acceleration readings that the program will take is equal to the ````SAMPLES```` variable.  
The number of x-acceleration readings that are equal to or greater than the threshold will be reported back via the serial connection followed by a newline character.
## Raw Mode
#### Config
Please set the ````RAW_TEST```` preprocessor variable to ````true```` and set the other
test mode variables (BATCH_TEST, BATCH_DETECTION_TEST, and DETECTION_TEST) to ````false````.
#### Method
In the raw test mode, the program will take an x-acceleration reading and report back the value in meters per second squared followed by a newline character via the Serial connection (see Configure Serial).  
If you would like for the program to instead take an average of multiple readings, please set the ````AVERAGE_RAW```` variable to ````true````. This will cause the prorgam to take a number of readings equal to the ````SAMPLES```` variable, average them, and report back the average followed by a newline character via the Serial connection.  
In both cases, the Serial Plotter function of the Arduino IDE should be able to be used.  

## Batch Detection Mode
#### Config
Please set the ````BATCH_DETECTION_TEST```` variable to ````true```` and set the other test mode variables to ````false````
#### Method
This test mode will cause the program to continuosly takes readings from the MMA and will send a message saying "DETECTED!" when the program reads a consecutive number of readings that are over the threshold given by the THRESHOLD variable.  
The number of consecutive readings needed is given by the ````IN_A_ROW```` variable.  

## Configure Threshold  
In the Batch and Detection tests, the program is using the ````THRESHOLD```` variable to determine if a value is equal to or greater than the threshold.  
The ````THRESHOLD```` value is always in meters per second squared.  
Please ensure that the sign (positive or negative) of your threshold value is correct.  

## Configure Serial
This test program has two ways to send Serial(text) messages:
- XBee (Attached to TX2 and RX2)
- USB Serial

By default, the program will send messages over USB serial.  
To make the program to send messages over the XBee instead, please change the ````USE_XBEE```` variable to ````true````.  
To make the program send messages over the USB, set the ````USE_XBEE```` variable to ````false````.  
To change the baud rate that the Serial connection runs at (applies to both USB and XBee), you may change the ````BAUD_RATE```` variable to the correct bits per second (bps) value. By default it is 9600 bps
