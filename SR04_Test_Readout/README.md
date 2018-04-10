# SR04 Test Program
This program is to test the SR04 ultrasonic distance sensor that will be mounted on the bottom of the Mars Lander.  
The program will be reporting values and messages over USB Serial.

The program will begin the serial connection with a "HELLO" message, and then proceed
to take readings from the ultrasonic sensor every ````SAMPLES_DELAY```` milliseconds.
It will then use this reading and past readings to calculate a rolling average of ````UTS_SAMPLES```` readings.  
It will also check to see if the rolling average is less than or equal to the threshold specified by the ````THRESHOLD```` variable.  
The distance reading from the ultrasonic sensor, the rolling average, and whether or not the threshold was met will all be reported in one message.  

The format of the USB message is: [distance cm], [rolling average distance], [threshold met (yes/no)]

### Units  
- ````THRESHOLD```` is in centimeters
- ````SAMPLES_DELAY```` is in milliseconds
- distance reported is in centimeters
- ````BAUD_RATE```` is in bits per second (bps)
