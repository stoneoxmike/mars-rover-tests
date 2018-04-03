# USB Test
The purpose of this program is to test USB Serial connectivity.  
During the program, messages will be sent down the USB connection in sequence.  
The format of the messages is as follows:  
````Test: <#>```` Where ````<#>```` is the message number.
After a message is sent, the message number is incremented by 1.  
After each message, the program delays for ````DELAY```` milliseconds.  
To adjust the baud rate of the serial connection, adjust the ````BAUD_RATE```` variable to the correct bits per second value.  
  
  
This test is very similar to the XBee Transmission Test. 
