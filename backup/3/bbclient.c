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
#include<semaphore.h>

int readFlag=0, listFlag=0,exitflag=0;
sem_t fileLock,threadLock;
//pthread_t tid;
//pid_t pid;
char userinput,storage[1];


/**
 *This is the thread function that is used by the thread
 */


void * userEntry(){
	int dummyReadFlag=0,dummyListFlag=0,dummyExitFlag=0;
	char userKey,blankResponse,userMessage[1024];
	while(i>0){
		printf("Please press a key:\nw - Write to the board \nr - Read from the board\n ");
		printf(" l- List the messages\n e - Exit the ring\n");
		scanf("%c",&userKey);
		scanf("%c",&blankResponse);
		
		if(userKey=='w'||userKey=='W'){
			printf("Enter Your message\nMessage should not exceed 1023 characters\n************************\n::");
			scanf("%[^\n]s",userMessage);
		}else if(userKey=='r'||userKey=='R'){
			printf("Please Enter the message number you want to read\n");
			scanf("%d",&dummyRead);
		}else if(userKey=='l'||userKey=='L'){
			printf("You have choosen to list all messages\n");
			dummyListFlag=1;			
		}else if(userKey=='e'||userKey=='E'){
			printf("You want to exit\n");
			dummyExitFlag=1;			
		}
		
		

		
	}
	
	return NULL;
}




int main(int argc, char ** argv){
     	time_t t;
	char *fileName;
	pthread_t tid;
	pid_t pid;
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
	if(argc!=5 && argc!=6){
		printf("Error\n Usage: <%s> <portNumber> <hostIP> <hostPort> <fileName> \n",argv[0]);
		printf("OR\n Usage: <%s> <new> <portNumebr> <hostIP> <hostPort> <fileName>\n",argv[0]);
		exit(1);
	}else if(argc==5){
	si_me.sin_port=(u_short)(atoi(argv[1]));
	printf("the port is %hu\n",si_me.sin_port);
	fileName=(argv[4]);
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
	


	//Initializing the semaphore lock
	sem_init(&fileLock,0,1);
	sem_init(&threadLock,0,1);

	//Creating user entry thread
	pthread_create(&tid, NULL,userEntry, NULL);

	//Main thread will continue to select the peer to get winning client
	//It will also initiate the token, and passes through the clients

	while(1){
		sendto(socketfd,&id, sizeof (&id), 0, (struct sockaddr *) &(peerInfo[0]), (socklen_t)slen);
		if(id>0){
			id++;
		}else if(id<0){
			id=0;
		}
		recvfrom(socketfd,&rcv,sizeof(&rcv),0,(struct sockaddr *)&si_other, (socklen_t*)&slen);

		if(rcv==clientID){
			flag=1;
			printf("This is the winning client\n Initiating token passing%d\n",flag);
			id=-3;
			//break;
		}else if(rcv==0){
			printf("Waiting to receive token\n");
		}else if(rcv<0){
			printf("I gain the token\n*******************\n");
					
			sleep(5);//it is doing work
			id=rcv;//Which means that after sending the token I am worthless 
		}
	}

//
//	if(flag==1){
//		printf("This is wining client\n");
//	}














        close(s);

















//*****************************************************er niche jaoa nished*************************************




	return 0;

}


