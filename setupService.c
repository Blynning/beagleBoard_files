#include "ipGetter.h"
#include "display.h"
#include "localTime.h"
#include "tempDev.h"
#include <stdio.h>
#include <unistd.h> //for usleep()
#include <stdlib.h> // for malloc()
#include <string.h> // for strlen()
#include <pthread.h> // for async


pthread_mutex_t display_mutex = PTHREAD_MUTEX_INITIALIZER;

void *fetch_ip(void *arg){

        int ip_isValid = 0;
        char* ipaddress_eth0 = malloc(15 * sizeof(char)); // allocate memory for ip address

        while(ip_isValid == 0){
                getIPv4(ipaddress_eth0); //try get ip address from "ipGetter.h"
                if(strcmp(ipaddress_eth0, "0") == 0){ //first char of ip is 0
                        ipaddress_eth0 = "loading";
                }else{
                        int length = strlen(ipaddress_eth0);
                        int i;
                        for (i = length - 1; i >= 0; i--) {
                                if (ipaddress_eth0[i] == '.') {
                                        if (ipaddress_eth0[i + 1] == '0') { // if there is a 0 after the last .
                                                ipaddress_eth0 = "loading";
                                                break;
                                        } else {
                                                ip_isValid = 1;
                                                break;
                                        }
                                }
                        }
                }
                pthread_mutex_lock(&display_mutex);
                display_write(ipaddress_eth0, 1);
                pthread_mutex_unlock(&display_mutex);
                sleep(2);
                        
        }
        free(ipaddress_eth0); //free allocated memory for the ip
}

void *display_temp_time(void *arg){
        int show_dots = 1;
        int update_timer_max_temp = 20;
        int update_timer_current_temp = 0;
        char* time_hour = malloc(3 * sizeof(char));
        char* time_minutes = malloc(3 * sizeof(char));
        char* temperature_dec= malloc(7*sizeof(char));
        char time_text[6];
        char temp_text[100];

        while(1){

                if(update_timer_current_temp <= 0)
                {
                        // hent temp
                        getTemp(temperature_dec);
                        update_timer_current_temp = update_timer_max_temp;
                }
                else{
                        update_timer_current_temp--;
                }

                getHour(time_hour, 2);

                getMin(time_minutes);
                
                if(show_dots == 1){
                        sprintf(time_text, "%s:%s", time_hour, time_minutes);
                }
                else{
                        sprintf(time_text, "%s %s", time_hour, time_minutes);
                }
                show_dots *= -1;

                char displayText[20];

                sprintf(displayText, "  %s  %s", time_text, temperature_dec);
                pthread_mutex_lock(&display_mutex);
                display_write(displayText, 2);
                pthread_mutex_unlock(&display_mutex);
                usleep(500000);<
                
        }
        free(time_hour);
        free(time_minutes);
        free(temperature_dec);
}


int main( void ) {
       pthread_t fetch_ip_thread, display_temp_time_thread;

        temp_init();
        display_init();
        
        // Create threads½½
        pthread_create(&fetch_ip_thread, NULL, fetch_ip, NULL);
        pthread_create(&display_temp_time_thread, NULL, display_temp_time, NULL);

        // Wait for threads to finish (which should never happen in this case)
        pthread_join(fetch_ip_thread, NULL);
        pthread_join(display_temp_time_thread, NULL);
      
  
        // Destroy mutex
        pthread_mutex_destroy(&display_mutex);
     
        return(0);
}