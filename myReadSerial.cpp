#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>

#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>

using namespace std;

int serialConfig(void);

int main(){
    int fd = serialConfig();
    std::string str;
    char read_buffer[1023];   /* Buffer to store the data received              */
    int  bytes_read = 0;    /* Number of bytes read by the read() system call */
    int count=0;
    while(true){
        bytes_read = read(fd, &read_buffer, 1023); /* Read the data                   */
        cout<<bytes_read<<endl;
        for(int i=0; i<bytes_read; i++)	 /*printing only the received characters*/
            printf("%c",read_buffer[i]);
        str = std::string(read_buffer);
    }
    return 0;
}

int serialConfig(void){
    int fd;/*File Descriptor*/

    /*------------------------------- Opening the Serial Port -------------------------------*/
    /* Change /dev/ttyUSB0 to the one corresponding to your system */
    fd = open("/dev/ttyUSB0",O_RDWR | O_NOCTTY);	/* ttyUSB0 is the FT232 based USB2SERIAL Converter   */
                        /* O_RDWR   - Read/Write access to serial port       */
                        /* O_NOCTTY - No terminal will control the process   */
                        /* Open in blocking mode,read will wait              */
                            
    if(fd == -1)						/* Error Checking */
        printf("\n  Error! in Opening ttyUSB0  ");
    else
        printf("\n  ttyUSB0 Opened Successfully ");

    /*---------- Setting the Attributes of the serial port using termios structure --------- */
    struct termios SerialPortSettings;	/* Create the structure                          */
    tcgetattr(fd, &SerialPortSettings);	/* Get the current attributes of the Serial port */

    /* Setting the Baud rate */
    cfsetispeed(&SerialPortSettings,B115200); /* Set Read  Speed as 9600                       */
    cfsetospeed(&SerialPortSettings,B115200); /* Set Write Speed as 9600                       */

    /* 8N1 Mode */
    SerialPortSettings.c_cflag &= ~PARENB;   /* Disables the Parity Enable bit(PARENB),So No Parity   */
    SerialPortSettings.c_cflag &= ~CSTOPB;   /* CSTOPB = 2 Stop bits,here it is cleared so 1 Stop bit */
    SerialPortSettings.c_cflag &= ~CSIZE;	 /* Clears the mask for setting the data size             */
    SerialPortSettings.c_cflag |=  CS8;      /* Set the data bits = 8                                 */
    SerialPortSettings.c_cflag &= ~CRTSCTS;       /* No Hardware flow Control                         */
    SerialPortSettings.c_cflag |= CREAD | CLOCAL; /* Enable receiver,Ignore Modem Control lines       */ 
    SerialPortSettings.c_iflag &= ~(IXON | IXOFF | IXANY);          /* Disable XON/XOFF flow control both i/p and o/p */
    SerialPortSettings.c_iflag &= ~(ICANON | ECHO | ECHOE | ISIG);  /* Non Cannonical mode                            */
    SerialPortSettings.c_oflag &= ~OPOST;/*No Output Processing*/
    
    /* Setting Time outs */
    SerialPortSettings.c_cc[VMIN] = 10; /* Read at least 10 characters */
    SerialPortSettings.c_cc[VTIME] = 0; /* Wait indefinetly   */


    if((tcsetattr(fd,TCSANOW,&SerialPortSettings)) != 0) /* Set the attributes to the termios structure*/
        printf("\n  ERROR ! in Setting attributes");
    else
        printf("\n  BaudRate = 115200 \n  StopBits = 1 \n  Parity   = none");
        
        /*------------------------------- Read data from serial port -----------------------------*/

    tcflush(fd, TCIFLUSH);   /* Discards old data in the rx buffer            */

    return fd;

    // char read_buffer[1023];   /* Buffer to store the data received              */
    // int  bytes_read = 0;    /* Number of bytes read by the read() system call */
    // int i = 0;

    // while(1){
    //     bytes_read = read(fd,&read_buffer,1023); /* Read the data                   */
    //     for(i=0;i<bytes_read;i++)	 /*printing only the received characters*/
    //         printf("%c",read_buffer[i]);
    // }
    // close(fd); /* Close the serial port */
}