#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <sys/time.h>
#include <time.h>
#include <errno.h>


int main(int argc, char *argv[])
{
	if(argc < 4) {
		printf("error");
		return 0;
	}
	struct sockaddr_in myaddr;
        struct sockaddr_in remaddr;
        socklen_t addrlen = sizeof(remaddr);	/* length of addresses */
        int recvlen;                    
        int fd;                         
	int port = atoi(argv[1]);
	struct timeval tv1, tv2, read_timeout;
	time_t t1,t2;
        /* create a UDP socket */

        if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
                perror("cannot create socket\n");
                return 0;
        }

        /* bind the socket to any valid IP address and a specific port */

        memset((char *)&myaddr, 0, sizeof(myaddr));
        myaddr.sin_family = AF_INET;
        myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
        myaddr.sin_port = htons(port);

        if (bind(fd, (struct sockaddr *)&myaddr, sizeof(myaddr)) < 0) {
                perror("bind failed");
                return 0;
        }
	int recvping;
	int err;
	read_timeout.tv_sec = atoi(argv[2]);
	read_timeout.tv_usec = 0;
        /* loop to recieve data and send back response */
        while(1) {
            
            //Make the recvfrom non-blocking
            err = setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, (const void *)&read_timeout, sizeof read_timeout);
            gettimeofday(&tv1, NULL);
            t1 = tv1.tv_sec;
            recvlen = recvfrom(fd, (char*)&recvping, sizeof(recvping), 0, (struct sockaddr *)&remaddr, &addrlen);
            //check for errno and call sleep and reset the awake time
            if((errno == EAGAIN || errno == EWOULDBLOCK) && recvlen < 0) {
                sleep(atoi(argv[3]));
                /* reset timeval struct after sleep */
                read_timeout.tv_sec = atoi(argv[2]);
        		read_timeout.tv_usec = 0;
            }
            gettimeofday(&tv2, NULL);
            t2 = tv2.tv_sec;
            read_timeout.tv_sec = read_timeout.tv_sec - (t2 - t1);
            read_timeout.tv_usec = 0;
            recvping = ntohl(recvping);
            if (recvlen > 0) {
                uint32_t sendmsg = 2;
                sendmsg = htonl(sendmsg);
                if (sendto(fd, (const char*)&sendmsg, sizeof(sendmsg), 0, (struct sockaddr *)&remaddr, sizeof(remaddr)) < 0) {
                    perror("sendto failed");
                    return 0;
                }
            }
        }
	
	return 0;
}
