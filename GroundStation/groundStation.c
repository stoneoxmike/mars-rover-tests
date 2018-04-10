#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>  /* File Control Definitions          */
#include <termios.h>/* POSIX Terminal Control Definitions*/
#include <unistd.h> /* UNIX Standard Definitions         */
#include <errno.h>  /* ERROR Number Definitions          */

/* Ground Station Modes. (What the groundstation is currently receiving) */
#define NONE 0  // Just logging functions or noise
#define DATA 1  // Image data that should be saved to the image file
#define SIZE 2  // A number describing the size of the image file

<<<<<<< HEAD

=======
#define READ_BUFFER_SIZE 10000
>>>>>>> a980561bbd4a1ad916033c1460c6bb79e25a215f

// termios = terminal input output settings
struct termios SerialPortSettings;

int fd; // This is the file descriptor for the current port.
char *portname = "/dev/ttyACM0"; // This is the location of the current serial port
FILE *imageFileHandle; // This is the file handle for the current image file

// Image processing variables
<<<<<<< HEAD
char read_buffer[513]; // The buffer than received bytes are read into
=======
char read_buffer[READ_BUFFER_SIZE]; // The buffer than received bytes are read into
>>>>>>> a980561bbd4a1ad916033c1460c6bb79e25a215f
char size_buffer[12]; // The buffer that ASCII characters describing the size of the image file are copied into
int lastSByte = 0; // The index of the size_buffer that the next ASCII character will be written to (push functionality, ask Tyler).
int bytes_read = 0; // The count of the number of bytes that were read into the read_buffer
long imageSize = 0; // The size of the image that is being/about to be received
long imageBytesRead = 0; // The number of bytes of the image file that have been received
int imageNumber = 0; // The number/ID of the image we are currently receiving (Image 0, 1, 2, 3), incremented once a full image has been received and written to the image file
int mode = NONE;
int bytes_processed = 0;

int connect_serial_port() {
	// A very useful tutorial that almost all of this is based off of: http://xanthium.in/Serial-Port-Programming-on-Linux
	fd = open(portname,O_RDWR | O_NOCTTY | O_SYNC);

	// Error handling
	if (fd < 0) {
		printf("Error opening %s: %s\n", portname, strerror(errno));
		return -1;
	}
	else {
	 	printf("%s Opened Successfully\n", portname);
	}

	//Now we will configure the settings for the serial port we just opened
	//Arduinos operate on 8N1 serial settings by default (note, this may be changed!)

	tcgetattr(fd, &SerialPortSettings); // Grab the current settings object
	cfsetispeed(&SerialPortSettings,B9600); //Set the reading baud rate
	cfsetospeed(&SerialPortSettings,B9600); //Set the writing baud rate

	//DO NOT TOUCH ANY OF THE SETTINGS BELOW THIS POINT. I WARNED YOU.
	SerialPortSettings.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off software based flow control
	SerialPortSettings.c_iflag &= ~(ICANON | ECHO | ECHOE | ISIG); // Switch to non-canonical mode.
	SerialPortSettings.c_cflag &= ~PARENB;  // Clear the parity setting (no parity)
	SerialPortSettings.c_cflag &= ~CSTOPB; //Stop bits = 1
	SerialPortSettings.c_cflag &= ~CSIZE; /* Clears the Mask       */
	SerialPortSettings.c_cflag |=  CS8;   /* Set the data bits = 8 */
	SerialPortSettings.c_cflag &= ~CRTSCTS; // We don't have the luxury of flow control so we must turn it off
	SerialPortSettings.c_cflag |= CREAD | CLOCAL;//enable receiver
	//We want a polling read, so that means as soon as data is available, read it
	SerialPortSettings.c_cc[VMIN]  = 0; /* Read as soon as we get a character */
	SerialPortSettings.c_cc[VTIME] = 0;  /* Don't ever time out   */

	tcsetattr(fd,TCSANOW,&SerialPortSettings); // Apply our changes to the settings
<<<<<<< HEAD

=======
	
>>>>>>> a980561bbd4a1ad916033c1460c6bb79e25a215f
	return fd;
}

int open_image_file() { // This will open and initialize our image file to be written to
	char fileName[6];
	memset(fileName, '\0', 6 * sizeof(char));
	char number[2];
	const char* extension = ".jpg";
	sprintf(number, "%d", imageNumber);
	memcpy(fileName, number, 1);
	strcat(fileName, extension);

 	imageFileHandle = fopen(fileName, "wb"); // Open a new image file (or clear the already existing one) and write to it in binary mode
 	if (imageFileHandle == NULL) {
		printf("Error opening file %s: %s\n", fileName, strerror(errno));
		return -1;
	}
	else {
	 	printf("%s Opened Successfully\n", fileName);
	}
}

void read_loop() {
	setbuf(stdout, NULL);
<<<<<<< HEAD
	memset(read_buffer, '\0', 513*sizeof(char)); // Clear the read buffer
	memset(size_buffer, '\0', 12*sizeof(char)); // Clear the size buffer
	mode = NONE;

	while(bytes_read < 1) {
		memset(read_buffer, '\0', 513*sizeof(char)); // Clear the read buffer so that only the newly read bytes are in the buffer
		bytes_read = read(fd,read_buffer,512); // Read the new bytes into the read buffer
=======
	memset(read_buffer, '\0', READ_BUFFER_SIZE*sizeof(char)); // Clear the read buffer
	memset(size_buffer, '\0', 12*sizeof(char)); // Clear the size buffer
	mode = NONE;
	
	while(bytes_read < 1) {
		memset(read_buffer, '\0', READ_BUFFER_SIZE*sizeof(char)); // Clear the read buffer so that only the newly read bytes are in the buffer
		bytes_read = read(fd,read_buffer,READ_BUFFER_SIZE-1); // Read the new bytes into the read buffer
>>>>>>> a980561bbd4a1ad916033c1460c6bb79e25a215f

		if(bytes_read > 0) { // if we got bytes, process them
			bytes_processed = 0; // Reset our processed bytes count
			while(bytes_processed < bytes_read) { // While we still have bytes remaining to process...
				if(mode == NONE) { // If we are in the NONE mode
					for(int i = bytes_processed; i < bytes_read; i++) { // For each byte we have to process, process it
						bytes_processed++; // We are processing one byte
						if(read_buffer[i] == '|') { // This byte is telling us that the next byte is part of the size description, so switch to size mode
							mode = SIZE; // Change the mode
							imageSize = 0; // Reset the image size
							imageBytesRead = 0; // Reset the number of image bytes read
							lastSByte = 0; // Reset the number of size bytes read
							memset(size_buffer, '\0', 12*sizeof(char)); // Reset the size buffer
							break; // Break out of the mode so that we can enter SIZE mode and process the size bytes
						}
					}
				}
				else if (mode == DATA) { // If we are expecting image data bytes
<<<<<<< HEAD
					for(int i = bytes_processed; (i < bytes_read) && (imageBytesRead < imageSize); i++) { // While we still have bytes to process and while the bytes we are processing are still image bytes
=======
				    long imageDelta = (imageSize - imageBytesRead); // This gives us the remaining # of bytes needed for the image file
				    long bytesToProcess = (bytes_read - bytes_processed); // This gives us the remaining number of bytes we have to process
				    if(bytesToProcess <= imageDelta) {
				        // The rest of the read buffer is image file data
				        fwrite(&read_buffer[bytes_processed], sizeof(read_buffer[0]) * bytesToProcess, 1, imageFileHandle);
				        imageBytesRead += bytesToProcess;
				        bytes_processed += bytesToProcess;
				    }
				    else {
				       // We will reach the end of the image file before the end of the read bytes
				       fwrite(&read_buffer[bytes_processed], sizeof(read_buffer[0]) * imageDelta, 1, imageFileHandle);
				       imageBytesRead += imageDelta;
				       bytes_processed += imageDelta;
				    }
/*					for(int i = bytes_processed; (i < bytes_read) && (imageBytesRead < imageSize); i++) { // While we still have bytes to process and while the bytes we are processing are still image bytes
>>>>>>> a980561bbd4a1ad916033c1460c6bb79e25a215f
						char write_buffer[1];
						write_buffer[0] = read_buffer[i];
						fwrite(write_buffer, sizeof(write_buffer), 1, imageFileHandle);
						imageBytesRead++; // Increment the number of bytes read
						bytes_processed++; // We processed a byte
<<<<<<< HEAD

					}
					if(imageBytesRead >= imageSize) {// Check to see if we have reached the end of the image file
=======
					
					}
*/					if(imageBytesRead >= imageSize) {// Check to see if we have reached the end of the image file
>>>>>>> a980561bbd4a1ad916033c1460c6bb79e25a215f
						imageNumber++; // Increment the image number
						mode = NONE; // Change the mode
						fclose(imageFileHandle); // Close the image file
					}
				}
				else if (mode == SIZE) { // Now we need to read in the size of the image
					for(int i = bytes_processed; i < bytes_read; i++) {
						if(read_buffer[i] == '|') { // End of the size number, switch mode and process the string to long for the size
							mode = DATA;
							imageSize = atol(size_buffer); // ASCII to long
							printf("Image size is: %ld Bytes\n", imageSize); // Print out the image size number that we got
							bytes_processed++; // We processed one byte
							// Ok now that we have switched to the data mode, lets open up our image file
							open_image_file();
							break;
						}
						else { // We were given a character (should be a character in the size number), so add it to the size_buffer
							size_buffer[lastSByte] = read_buffer[i]; // Set the current next character in the size buffer to the byte we are processing
							lastSByte++; // Increment the position of the next character in the size buffer
							bytes_processed++; // We processed one byte
						}
					}
				}
<<<<<<< HEAD

=======
		
>>>>>>> a980561bbd4a1ad916033c1460c6bb79e25a215f
			}
			bytes_read = 0;
		}
	}
}

int main()
{
  fd = connect_serial_port();
<<<<<<< HEAD

=======
  
>>>>>>> a980561bbd4a1ad916033c1460c6bb79e25a215f
/*
  char write_buffer[1] = "B";
  int  bytes_written  =  0 ;
  bytes_written = write(fd,write_buffer,sizeof(write_buffer));
usleep(100);
*/

  read_loop(); // Enter the read loop (waiting for image transmission).
  close(fd); // Close the serial connection
  return 0;
}
