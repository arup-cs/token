#include<stdio.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include<netdb.h>
#include<string.h>
#include<stdlib.h>
int main(int argc, char ** argv){
	int portnumber,hostCount;
	
	if(argc!=3){
		printf("Error\nUSAGE: <%s> <PORT_NUMBER> <NUMBER_OF_HOST>\n",argv[0]);
	exit(0);
	}else{
		portnumber=atoi(argv[1]);
		hostCount=atoi(argv[2]);
		if(hostCount<3){
			printf("Minimum 3 clients are required to create the ring\n");
			exit(0);
		
		}
	}


	//declaration 
	struct sockaddr_in si_me, si_other[hostCount];
	socklen_t slength=sizeof(si_other);
        int serversocket=0, i=0, slen=sizeof(si_other[0]);
        char buf[1024]="This is server buffer";
	
	//creating socket
        if ((serversocket=socket(AF_INET, SOCK_DGRAM, 0))==-1)
	{
         	printf("Error in creating socket\n");
	}
    
	//Filling server address
        memset((char *) &si_me, 0, sizeof(si_me));
        si_me.sin_family = AF_INET;
        si_me.sin_port = htons(portnumber);
	si_me.sin_addr.s_addr = inet_addr("127.0.0.1");
	printf("The IP Address %s %d is : %s\n",buf,i,inet_ntoa(si_me.sin_addr));
	

	//binding socket
	bind(serversocket, (struct sockaddr *) &si_me, (socklen_t)slen);
	//receiving data from client
	char ackbuffer[1024]="Messege Delivered to server";
	for(i=0;i<hostCount;i++){
		printf("Waiting to receive from client %d\n",i+1);
		recvfrom(serversocket,buf,sizeof(buf),0,(struct sockaddr *)&si_other[i], (socklen_t *)&slength);
		printf("Client Info:\n");
		printf("IP ADDRESS: %s\n",inet_ntoa(si_other[i].sin_addr));
		printf("PORT: %hu\n_______________________________\n",si_other[i].sin_port);
		printf("Data received[+]: %s\n",buf);
		sendto(serversocket,ackbuffer,sizeof(ackbuffer),0,(struct sockaddr *)&si_other[i],(socklen_t)slength);
		printf("Ack sent\n***********************************\n");
	}
	


	//Sending ring formation info
	int nextInfo=0;
	for(i=0;i<hostCount;i++){
		nextInfo=i+1;
		if(nextInfo==hostCount){
			nextInfo=0;
		}
		sendto(serversocket,&si_other[nextInfo],sizeof(si_other[nextInfo]),0,(struct sockaddr *)&si_other[i],(socklen_t)slength);
		

	}
	

























//**************************er niche jaoa nished**********************************
	return 0;

}
