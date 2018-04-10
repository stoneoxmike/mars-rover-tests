#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>  /* File Control Definitions          */
#include <termios.h>/* POSIX Terminal Control Definitions*/
#include <unistd.h> /* UNIX Standard Definitions         */
#include <errno.h>  /* ERROR Number Definitions          */

// A very useful tutorial that almost all of this is based off of: http://xanthium.in/Serial-Port-Programming-on-Linux

// termios = terminal input output settings

struct termios SerialPortSettings;

int main()
{
  int fd;
 char *portname = "/dev/ttyACM0";
  fd = open(portname,O_RDWR | O_NOCTTY | O_SYNC);

  if (fd < 0) {
        printf("Error opening %s: %s\n", portname, strerror(errno));
        return -1;
  }

  if(fd == 1) {
   printf("\n  Error! in Opening ttyUSB0\n");
  }
  else {
     printf("\n  ttyUSB0 Opened Successfully\n");
  }
  //Now we will configure the settings for the serial port we just opened
  //Arduinos operate on 8N1 serial settings by default (note, this may be changed!)

  tcgetattr(fd, &SerialPortSettings); // Grab the current settings object
  cfsetispeed(&SerialPortSettings,B9600); //Set the reading baud rate
  cfsetospeed(&SerialPortSettings,B9600); //Set the writing baud rate

  //DO NOT TOUCH ANY OF THE SETTINGS BELOW THIS POINT. I WARNED YOU.
  SerialPortSettings.c_iflag &= ~(IXON | IXOFF | IXANY);
  SerialPortSettings.c_iflag &= ~(ICANON | ECHO | ECHOE | ISIG);
  SerialPortSettings.c_cflag &= ~PARENB;  // Clear the parity setting (no parity)
  SerialPortSettings.c_cflag &= ~CSTOPB; //Stop bits = 1
  SerialPortSettings.c_cflag &= ~CSIZE; /* Clears the Mask       */
  SerialPortSettings.c_cflag |=  CS8;   /* Set the data bits = 8 */
  SerialPortSettings.c_cflag &= ~CRTSCTS; // We don't have the luxury of flow control so we must turn it off
  SerialPortSettings.c_cflag |= CREAD | CLOCAL;//enable receiver
  SerialPortSettings.c_cc[VMIN]  = 0; /* Read as soon as we get a character */
  SerialPortSettings.c_cc[VTIME] = 0;  /* Don't ever time out   */
 
 tcsetattr(fd,TCSANOW,&SerialPortSettings); // Apply our changes to the settings

  char write_buffer[1] = "B";
  int  bytes_written  =  0 ;
  bytes_written = write(fd,write_buffer,sizeof(write_buffer));
usleep(100);

char read_buffer[513];
int  bytes_read = 0;
memset(read_buffer, '\0', 513*sizeof(char));

	while(bytes_read < 1) {
		memset(read_buffer, '\0', 513*sizeof(char));
		bytes_read = read(fd,&read_buffer,512);
		if(bytes_read > 0) {
			printf(read_buffer);
			printf("\n");
			bytes_read = 0;
		}
	}

  close(fd);
  return 0;
}
