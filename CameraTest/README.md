# Camera Test
This program is designed to test the PIL-9 JPEG cameras on the Mars Lander for image capture.  
In the final lander, there will be 4 cameras pointed in perpendicular directions.  
The cameras should each capture an image and have the image copied over to a file of the SD card.  

The micro SD card shield must be installed and configured correctly (see SD_Card_Test for more info).

# Config
This program is designed to test from one to four cameras on an Arduino MEGA. 
This program is also designed for multiple pin numberings to be tested/used (see Pin Configuration).

To test a camera, please set it's respective variable from ````false```` to ````true````.   
For example, if I wanted to test the camera connected to the pins specified by ```CAM_1_TX_PIN```` and ````CAM_1_RX_PIN````, I would change ````CAM_1_ENABLED```` to ````true````.

This may be tedious enabling each camera, so to save time, you can enable all 4 cameras by changing the ````ALL_CAMS_ENABLED```` variable to ````true````.

Note: If a camera is enabled and is not plugging in properly or not to the correct pins, the program will generate an error message and stop.  

# Pin Configuration
This program allows for flexible pin allocation, but please note the pins should adhere to the capabilities of SoftwareSerial.  

To set the pin number for Camera 1, change the following variables: ````CAM_1_RX_PIN```` and ````CAM_1_TX_PIN````
The RX pin number should be the pin number that runs to the pin on the JPEG camera labeled "RX".  
The TX pin number should be the Arduino MEGA pin number that runs to the pin on the JPEG camera labeled "TX".  
