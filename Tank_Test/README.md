# Tank Test
This program is designed to test the tank pairing and piloting functionality of the Mars Lander and Rover/Tank.  
During the test, the tank should drive forward, turn its turrent to drop the market, spin right, and drive forward again and stop.  
The test can also be configured to test the servo release of the tank (see Servo Release Testing).  


## Tank Piloting
The program will delay for 2 seconds, pair with the tank, pause a second, drive the tank forward for ````FORWARD_TIME```` milliseconds
and then stop the tank, turn the turrent to the left for ````TURRENT_TIME```` milliseconds, and the then the turrent right for ````TURRENT_TIME```` milliseconds.
The tank will then be piloted to spin right for ````SPIN_TIME```` milliseconds, and then drive forward for ````FORWARD_TIME```` milliseconds.

## Tank Controller Pin Outs
Currently, the tank controller has 8 different pins used to control the Tank.  
These pins are connected to the Arduino MEGA for the Arduino to control the Tank.  
In order to control the tank, the program runnig on the Arduino must know what pins connect to the tank controller and what each connection controls.  
So there are eight variables that hold values representing the pins on the Arduino MEGA.  
Each variable is named according to what its connection controls:  
- ````TANK_LF_PIN```` -- Left Forward. When this pin is set to HIGH, the left tread on the tank will spin forwards.
- ````TANK_RF_PIN```` -- Right Forward.
- ````TANK_LB_PIN```` -- Left Backward. When this pin is set to HIGH, the left tread on the tank will spin backwards.
- ````TANK_RB_PIN```` -- Right Backward.
- ````TANK_TR_PIN```` -- Turrent Right. When this pin is set to HIGH, the turrent will spin the right.
- ````TANK_TL_PIN```` -- Turrent Left.
- ````TANK_POWER_PIN```` -- Tank Power Pin, used for pairing with the tank.
- ````TANK_FUNC_PIN````` -- Tank Function Pin, used for pairing with the tank.

## Servo Release Testing
The test can be configured to test the tank release servo on the Mars Lander.  
It will test putting the servo in "locked" and "released" positions.  
To enable Servo Release Testing, please change the ````RELEASE_TEST```` variable to ````true````.
  
  
**Please have the tank powered on and inside of the Mars Lander before running the test.**
