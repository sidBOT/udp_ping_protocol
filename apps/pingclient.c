#include <stdio.h>
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h>
#include <netdb.h>
#include <time.h>
#include <sys/time.h>
int main(int argc, char *argv[]){
	

	int fd;
	struct sockaddr_in myaddr;
	struct hostent *hp;
	struct timeval t1, t2, r1;
	int port = atoi(argv[1]);
	int recvping;
	uint32_t ping = 1;

        if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
                perror("cannot create socket");
                return 0;
        }

	memset((char *)&myaddr, 0, sizeof(myaddr));
	myaddr.sin_family = AF_INET;
	myaddr.sin_port = htons(port);
	int i;
	for(i = 2; i < argc; i++) {
		hp = gethostbyname(argv[i]);
        
		memcpy((void *)&myaddr.sin_addr, hp->h_addr_list[0], hp->h_length);
		ping = htonl(ping);	
		gettimeofday(&t1, NULL);
        //send a ping signal to the server
		if (sendto(fd, (const char*)&ping, sizeof(ping), 0, (struct sockaddr *)&myaddr, sizeof(myaddr)) < 0) {
			perror("sendto failed");
			return 0;
		}
		socklen_t addrlen = sizeof(myaddr);
        //recieve the signal from the server
		int recvlen = recvfrom(fd, (char*)&recvping, sizeof(recvping), 0, (struct sockaddr *)&myaddr, &addrlen);
	
		gettimeofday(&t2, NULL);
		timersub(&t2, &t1, &r1);
        //get latency
		recvping = htonl(recvping);	
		//printf("rec: %d\n", recvping);
		char *ip = inet_ntoa(myaddr.sin_addr);
		long latency = t2.tv_usec - t1.tv_usec;
		if(latency < 0) latency * -1;
		printf("Reply from server %s, latency : %ldusec", ip, r1.tv_usec);
		printf("\n");
	}
	return 0;

}
