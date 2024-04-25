#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int getHour (char *timeH, int utcTime)
{
  time_t rawtime;
  struct tm * timeinfo; //declare pointer to struct

  time(&rawtime);

  timeinfo = localtime(&rawtime);

  sprintf(timeH, "%02d",timeinfo->tm_hour + utcTime);

  return(0);
}

int* getMin (char *timeM)
{
  time_t rawtime;
  struct tm * timeinfo; //declare pointer to struct

  time(&rawtime);
  timeinfo = localtime(&rawtime);
  sprintf(timeM, "%02d",timeinfo->tm_min);
  return(0);
}

