#include <stdio.h>
#include <unistd.h>
#include <string.h> /* for strncpy */
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>

// hent IPv4 addresse fra "eth0"
int get_IPv4(char *ipAdd)
{
    int fd; //socket fil descriptor (socket id)
    struct ifreq ifr; // struct for interface request

    //opret socket, i Ipv4 og Datagram "mode"
    fd = socket(AF_INET, SOCK_DGRAM, 0);

    //specify IPv4 address (AF_INET = IPv4)
    ifr.ifr_addr.sa_family = AF_INET;

    /* specify "eth0" interface*/
    strncpy(ifr.ifr_name, "eth0", IFNAMSIZ-1);

    //hent ip addressen fra det specificerede interface
    ioctl(fd, SIOCGIFADDR, &ifr);

    //luk for socket
    close(fd);

    // gem resultat i ipAdd
    // 1. cast ifr_addr(interface addr) til pointer (sockaddr_in)
    // 2. sin_addr har Ipv4 adressen
    // 3. inet_ntoa() konvertere ip adressen fra bits til decimal (000.000.000.000)
    sprintf(ipAdd, "%s", inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr));
    return 0;
}
