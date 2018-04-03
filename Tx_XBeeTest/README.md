# XBee Transmission Test
The purpose of this test is to test the transmission functionality of the XBee radio module on the Mars Lander.
During the test, a series of messages are sent in a sequence that is as follows:  
````LOGTest: <#>|````  
Where <#> is the current message count. Each time a message is sent, the message count is incremented by 1.  
After a message is sent, the program delays for ````DELAY```` milliseconds.  
To adjust the baud rate of the serial connection, change the ````BAUD_RATE```` variable to the correct bits per second value.

