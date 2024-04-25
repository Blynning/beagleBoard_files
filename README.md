
# Display.C (API For Grove-LCD RGB Backlight)

## endpoints
- display_init()
- display_write(char* message, int line)
  
## Important

- Run: display_init() FIRST - to initialize the display
- display_write(char* message, int line) default writes to 1st line. only "2" wil write to 2nd line

# display examples
```C
char displayText[20];
// - add text to displayText
// write the message to line 2
display_write(displayText, 2); 
```
# ipGetter.C (API to get IPv4 address from eth0 interface

## endpoints
- getIPv4(char *ipAdd)

## Important
- remember to allocate memory for the address. (see ip example1)
- see ip example 2 on how to get ip

### ip examples
```C
//example1:
char* ipaddress_eth0 = malloc(15 * sizeof(char)); // allocate memory for ip address
// get ip
getIPv4(ipaddress_eth0);
```

# localTime.C (Api to get current time)

## endpoints
- getHour (char *timeH, int utcTime)
- getMin (char *timeM)

## Important
- getHour(char *timeH, int utcTime) gets the hour in utc+0. utcTime adds the amount to the hour. (see localTime example1)
- size is always 3 * sizeof(char)

### localTime examples
```c
//example1, get hours:
char* time_hour = malloc(3 * sizeof(char));
getHour(time_hour, 2);
```
```c
//example2, get minutes:
char* time_minutes = malloc(3 * sizeof(char));
getMin(time_minutes);
```

#tempDev (APi to read temperature from I2C High accuracy temperature sensor)

## endpoints
- temp_init() initialize temperature sensor
- getTemp(char* tempAddr) reads temperature

## Important
- temperature is in format "%0.1fC" = 23.4C
- remember to allocate memory for the temperature. (see temp example1)

### temp examples
```c
//example1, allocate memory for temperature:
char* temperature_dec= malloc(7*sizeof(char));
//read temperature
getTemp(temperature_dec);
```

# Appenix
### to use malloc() and free()
```c
#include <stdlib.h>
```
### remember to free the memory allocation when done
```c
free(time_hour);
free(time_minutes);
free(temperature_dec);
```
