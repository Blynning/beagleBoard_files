#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

#define I2C_BUS "/dev/i2c-2"    // Path to the I2C device
#define LCD_ADDRESS 0x3e        // Address of the LCD module

// Function to send command to LCD
void lcd_command(int fd, unsigned char command) {
    unsigned char buf[2];
    buf[0] = 0x80;    // Control byte for command
    buf[1] = command;
    if (write(fd, buf, 2) != 2) {
        perror("Write failed");
        exit(1);
    }
    usleep(1000); // Delay for command execution
}

// Function to send data to LCD
void lcd_data(int fd, unsigned char data) {
    unsigned char buf[2];
    buf[0] = 0x40;    // Control byte for data
    buf[1] = data;
    if (write(fd, buf, 2) != 2) {
        perror("Write failed");
        exit(1);
    }
    usleep(1000); // Delay for data write
}

// Function to initialize LCD
void lcd_init(int fd) {
    // Initialization sequence
    lcd_command(fd, 0x38); // 8-bit, 2-line mode
    lcd_command(fd, 0x39); // Enable 4-bit mode
    lcd_command(fd, 0x14); // Internal OSC frequency
    lcd_command(fd, 0x73); // Contrast set
    lcd_command(fd, 0x56); // Power/ICON control/Contrast set
    lcd_command(fd, 0x6c); // Follower control
    usleep(200000);        // Wait for power stable
    lcd_command(fd, 0x38); // Function set (8-bit)
    lcd_command(fd, 0x0c); // Display control
    lcd_command(fd, 0x01); // Clear display
    usleep(2000);          // Wait for command execution
}

// Function to clear LCD display
void lcd_clear(int fd) {
    lcd_command(fd, 0x01);
    usleep(2000); // Wait for command execution
}

int main() {
    int i2c_fd;

    // Open the I2C bus
    if ((i2c_fd = open(I2C_BUS, O_RDWR)) < 0) {
        perror("Failed to open the i2c bus");
        exit(1);
    }

    // Set the I2C slave address
    if (ioctl(i2c_fd, I2C_SLAVE, LCD_ADDRESS) < 0) {
        perror("Failed to set the address");
        exit(1);
    }

    // Initialize LCD
    lcd_init(i2c_fd);

    // Write "Hello world" to LCD
    char hello[] = "Hello world";
    for (int i = 0; i < strlen(hello); i++) {
        lcd_data(i2c_fd, hello[i]);
    }

    // Close the I2C bus
    close(i2c_fd);

    return 0;
}
