#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h> 

int main(int argc, char *argv[])
{
    int sockfd = 0, n = 0;
    char recvBuff[1024];
    struct sockaddr_in serv_addr; 

    if(argc != 3)
    {
        printf("\n Usage: %s <ip of server> <port>\n",argv[0]);
        return 1;
    } 

    memset(recvBuff, '0',sizeof(recvBuff));
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Error : Could not create socket \n");
        return 1;
    } 

    memset(&serv_addr, '0', sizeof(serv_addr)); 

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(argv[2])); 

    if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr)<0)
    {
        printf("\n inet_pton error occured\n");
        return 1;
    } 

    if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
       printf("\n Error : Connect Failed \n");
       printf("errno = %s\n", strerror(errno));
       return 1;
    } 


    FILE* fd = fopen("matvec.c", "r");
    char buf[1024];
    int ch;
    while((ch = fread(buf, 1, 1024, fd)) == 1024){
	write(sockfd, buf, 1024);
    }
    write(sockfd, buf, ch);
    buf[0] = '\0';
    sleep(1);
    write(sockfd, buf, 1);
    fclose(fd);

fd = fopen("matvec.cl", "r");
    while((ch = fread(buf, 1, 1024, fd)) == 1024){
	write(sockfd, buf, 1024);
    }
    write(sockfd, buf, ch);
    buf[0] = '\0';
    sleep(1);
    write(sockfd, buf, 1);



	while((ch = read(sockfd, buf, 1024)) != 1){
		fwrite(buf, ch, 1, stdout);
	}


    fclose(fd);
    

    return 0;
}
