#include <sys/socket.h>

int sockfd = Socket(AF_INET, SOCK_STREAM,0 );
if(sockfd < 0){
	perror("socket");
	exit(1);
}