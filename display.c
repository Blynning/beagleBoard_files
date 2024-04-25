#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <linux/i2c-dev.h>
#include <i2c/smbus.h>        // Added 01062021 HeTh

#define I2C_BUS "/dev/i2c-2"  // Path to the I2C device
#define LCD_ADDRESS 0x3e      // Address of the LCD module
#define DEVICE_ADDRESS 0x62   //LED?
int i2c_fd;



// Function to send command to LCD
void lcd_command(unsigned char command) {
    unsigned char buf[2];
    buf[0] = 0x80;    // Control byte for command
    buf[1] = command; // command to execute / bit to change
    if (write(i2c_fd, buf, 2) != 2) // write to display, 2 is size
    {
        perror("Write failed");
        exit(1);
    }
}

// Function to send data to LCD
void lcd_data(unsigned char data) {
    unsigned char buf[2]; // data to Display
    buf[0] = 0x40; // Control byte for data
    buf[1] = data; // Char to display
    if (write(i2c_fd, buf, 2) != 2) // write to display, 2 is size
    {
        perror("Write failed");
        exit(1);
    }
}

int display_init() 
{

    // Open the I2C bus
    if ((i2c_fd = open(I2C_BUS, O_RDWR)) < 0) {
        perror("Failed to open the i2c bus");
        exit(1);
    }

    // Set the I2C slave address (ioctl = input / output, Control)
    if (ioctl(i2c_fd, I2C_SLAVE, DEVICE_ADDRESS ) < 0) {
        perror("Failed to set the address");
        exit(1);
    }

    // turn off sleepmode
    unsigned char data1[2] = {0x00, 0x00};
    write(i2c_fd, data1, 2);
    // set screen color to white
    unsigned char data2[2] = {0x08, 0x15};
    write(i2c_fd, data2, 2);

    // Set the I2C slave address (ioctl = input / output, Control?)
    if (ioctl(i2c_fd, I2C_SLAVE, LCD_ADDRESS) < 0) {
        perror("Failed to set the address");
        exit(1);
    }

     // Initialization sequence
    lcd_command(0x38); // 8-bit, 2-line mode
    lcd_command(0x39); // Enable 4-bit mode
    lcd_command(0x0c); // Display control
    lcd_command(0x01); // Clear display
    lcd_command(0x38); // Function set (8-bit)
    lcd_command(0x6c); // Follower control
    lcd_command(0x14); // Internal OSC frequency

    return(0);
}


void display_write(char* message, int line) 
{
    if(line == 2){
        // change line to 2
        unsigned char data1[2] = {0x00, 0xC0};
        write(i2c_fd, data1, 2);
    }
    
    /*else{
         // change line to 1
        unsigned char data1[2] = {0x00, 0x80};
        write(i2c_fd, data1, 2);
    }*/
        
    
    for (int i = 0; i < strlen(message); i++) {
        lcd_data(message[i]);
    }
}