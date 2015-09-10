#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h> 

int main(int argc, char *argv[])
{
    int listenfd = 0, connfd = 0;
    struct sockaddr_in serv_addr; 
    if(argc < 2){
		printf("./server <port>\n");
	}
    char sendBuff[1025];
    time_t ticks; 

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
	if(!listenfd){printf("fd\n"); exit(0);}
    memset(&serv_addr, '0', sizeof(serv_addr));
    memset(sendBuff, '0', sizeof(sendBuff)); 

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1])); 

    int ret = bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)); 
	if(ret){printf("bind\n"); exit(0);}

    ret = listen(listenfd, 10); 
	if(ret){printf("listen\n"); exit(0);}


    char cfile[16384];
    char clfile[16384];
    char buf[1024];
    int ch;
    
        connfd = accept(listenfd, (struct sockaddr*)NULL, NULL); 
printf("accept c! \n");
	int index = 0;
	while((ch = read(connfd, buf, 1024)) != 1){
		memcpy(&cfile[index], buf, ch);
		index += ch;
	}
	//printf("%s\n", cfile);
	FILE* fd = fopen("matvec.c", "w");
	fwrite(cfile, index, 1, fd);
	fclose(fd);

	system("gcc matvec.c -I/opt/intel/opencl-sdk/include/ -L/opt/intel/opencl-sdk/lib64/ -lOpenCL 2> /dev/null");

printf("accept cl! \n");
	index = 0;
	while((ch = read(connfd, buf, 1024)) != 1){
		memcpy(&clfile[index], buf, ch);
		index += ch;
	}
	//printf("%s\n", clfile);
	fd = fopen("matvec.cl", "w");
	fwrite(clfile, index, 1, fd);
	fclose(fd);

printf("running service \n");
	system("LD_PRELOAD=/opt/intel/opencl-sdk/lib64/libOpenCL.so ./a.out > log");
	fd = fopen("log", "r");
    while((ch = fread(buf, 1, 1024, fd)) == 1024){
	write(connfd, buf, 1024);
    }
    write(connfd, buf, ch);
    buf[0] = '\0';
    sleep(1);
    write(connfd, buf, 1);

/*	if((local = fork()) == -1){
		dup2(connfd, result[1]);
		close(result[0]);
	}
	else{
		dup2(1, result[0]);
		close(result[1]);
		char* args[] = {"/home/scope/a.out", NULL};
		char* envp[] = {"LD_PRELOAD=/opt/intel/opencl-sdk/lib64/libOpenCL.so", NULL};
		execve("/home/scope/socket/a.out", args, envp);
	}*/


        close(connfd);
        sleep(1);
     
}

