#include<stdlib.h>
#include<stdio.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include<string.h>
#include<time.h>
#include<pthread.h>

pthread_t tid;
pid_t pid;
char userinput,storage[1];


/**
 *This is the thread function that is used by the thread
 */


void * threadfunc(){
	printf("Getting the thread function\n");
	printf("The character is %c",storage[0]);	
	
	return NULL;
}




int main(int argc, char ** argv){
     	time_t t;
	int socketfd=socket(AF_INET,SOCK_DGRAM,0);
	srand((unsigned int) time(&t));

        if(socketfd==-1){
                printf("error\n");
                exit(0);
        }else{
	printf("The socket return %d\n",socketfd);
	}

        struct sockaddr_in si_other,si_me;
        int s=0;
	socklen_t slen=sizeof(si_other);
        char buffer[1024]="hello world";

//*************************************argument function **************
	if(argc==5){
	si_me.sin_port=(u_short)(atoi(argv[1]));
	printf("the port is %hu\n",si_me.sin_port);
	

	}



	printf("Satisfying compiler%d %d %s\n",s,slen,buffer);
//******i***************************************************************
/*
	int socketfd=socket(AF_INET,SOCK_DGRAM,0);
	if(socketfd==-1){
		printf("error\n");
		exit(0);
	}
	struct sockaddr_in si_other;
        int s, i, slen=sizeof(si_other);
        char buffer[1024];
  */     
	memset((char *) &si_other, 0, sizeof(si_other));
        si_other.sin_family = AF_INET;
        si_other.sin_port = htons(60000);
	si_other.sin_addr.s_addr=inet_addr("127.0.0.1");    
//	buffer='Hello server';
   /* 	
	if (inet_aton(SRV_IP, &si_other.sin_addr)==0) {
          fprintf(stderr, "inet_aton() failed\n");
          exit(1);
        }*/
    
        /*
          if(sendto(s, buffer,sizeof(buffer), 0,(struct sockaddr *) &si_other, (socklen_t)slen)==-1){
		printf("Sending failed");
}
*/
//	connect(s,(struct sockaddr*)&si_other,sizeof(si_other));
//	send(s,buffer,1024,0);  
	sendto(socketfd, buffer, sizeof (buffer), 0, (struct sockaddr *) &si_other, (socklen_t)slen);
	printf("Data sent\n");
	recvfrom(socketfd,buffer, sizeof(buffer),0,(struct sockaddr *)&si_other, (socklen_t*)&slen);
	printf("Acknowdledge[-]: %s\n",buffer);
    
	//Getting info about its peer: 
	struct sockaddr_in peerInfo[2];
	int i;
	for(i=0;i<1;i++){
	recvfrom(socketfd,& (peerInfo[i]), sizeof(peerInfo[i]),0,(struct sockaddr *)&si_other, (socklen_t*)&slen);
		if(i==0){
			printf("My next peer's info\n");
			printf("IP Address: %s\n",inet_ntoa(peerInfo[i].sin_addr));
			printf("Port: %hu\n",peerInfo[i].sin_port);
		}
	}
	
	//Playing game to select only one peer
	//Creting random ID for each client
	
	int id=1,rcv,flag=0;
	//time_t t;
	int clientID;
	
	int sub= rand()%1571;
	clientID=abs((int) getpid()-sub);
	printf("Client ID is: %d and sub is%d\n",clientID,sub);
	
	//struct sockaddr_in ringinfo;
//	printf("Waiting to get the wining client\n");
	pid=fork();
	if(pid==0){
		printf("Enter a character:\n");
		scanf("%c",&userinput);
		storage[0]=userinput;
		printf("Thank you for inputting %c\n",storage[0]);
	//	flush(userinput);
	}else{
//starting else++++++++++++++++++++++++++++++++++++++++++++++++++
	

	while(1){
		sendto(socketfd,&id, sizeof (&id), 0, (struct sockaddr *) &(peerInfo[0]), (socklen_t)slen);
		if(id>0){
			id++;
		}else if(id<0){
			id=0;
		}
		recvfrom(socketfd,&rcv,sizeof(&rcv),0,(struct sockaddr *)&si_other, (socklen_t*)&slen);
	//	printf("RECEIVED INT INFO: %d\n",rcv);
		if(rcv==clientID){
			flag=1;
			printf("This is the winning client\n Initiating token passing%d\n",flag);
			id=-3;
			//break;
		}else if(rcv==0){
			printf("I havenot receive token\n");
		}else if(rcv<0){
			printf("I gain the token\nstarting my work\n");
				pthread_create(&tid, NULL, threadfunc, NULL);
		
			sleep(5);//it is doing work
			id=rcv;//Which means that after sending the token I am worthless 
		}
	}

}    //ending else+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//	if(flag==1){
//		printf("This is wining client\n");
//	}














        close(s);

















//*****************************************************er niche jaoa nished*************************************




	return 0;

}


