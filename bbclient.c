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

<<<<<<< HEAD
int readFlag=0, listFlag=0,exitFlag=0,newClientFlag=0;
=======

int readFlag=0, listFlag=0,exitFlag=0;
>>>>>>> c06bdea0a15d0ec8757471142d2267c1e94cbf7f
sem_t fileLock,threadLock;
char userinput,storage[1],globalMessage[1024];


/**
 *This is the thread function that is used by the thread
 */


void * userEntry(){
	int dummyReadFlag=0,dummyListFlag=0,dummyExitFlag=0;
	char userKey,blankResponse,userMessage[1024];
	
	//Running loop to get the user response repeatedly
	while(dummyExitFlag!=1){
		printf("Please press a key:\nw - Write to the board \nr - Read from the board\n ");
		printf(" l- List the messages\n e - Exit the ring\n");
		scanf("%c",&userKey);
		scanf("%c",&blankResponse);
		
		if(userKey=='w'||userKey=='W'){
			printf("Enter Your message\nMessage should not exceed 1023 characters\n************************\n::");
			scanf("%[^\n]s",userMessage);
			scanf("%c",&blankResponse);
		}else if(userKey=='r'||userKey=='R'){
			printf("Please Enter the message number you want to read\n");
			scanf("%d",&dummyReadFlag); 
			scanf("%c",&blankResponse);
		}else if(userKey=='l'||userKey=='L'){
			printf("You have choosen to list all messages\n");
			dummyListFlag=1;			
		}else if(userKey=='e'||userKey=='E'){
			printf("You want to exit\n");
			dummyExitFlag=1;			
		}
		
		//Updating the global variable that will be read and modified by the main thread
		size_t destination_size=sizeof(globalMessage);
		sem_wait(&threadLock);
		strncpy(globalMessage,userMessage, destination_size); 
		globalMessage[destination_size - 1] = '\0';
		readFlag=dummyReadFlag;
		listFlag=dummyListFlag;
		exitFlag=dummyExitFlag;
		
		//Unlocking the lock used for the update
		sem_post(&threadLock);
		
		//clearing the local variable
		memset(userMessage, 0, sizeof (userMessage));
		userMessage[0]='\0';
		dummyReadFlag=0;
		dummyListFlag=0;
			
				
	}
	
	return NULL;
}




int main(int argc, char ** argv){
     	time_t t;
	FILE * filePointer;
	char *fileName;
	pthread_t tid;
	pid_t pid;
	int socketfd=socket(AF_INET,SOCK_DGRAM,0);
	

        if(socketfd==-1){
                printf("error\n");
                exit(0);
        }else{
	printf("The socket return %d\n",socketfd);
	}

        struct sockaddr_in si_other,si_me;
        int s=0,position;
	socklen_t slen=sizeof(si_other);
        char buffer[1024]="hello world";
	memset((char *) &si_other, 0, sizeof(si_other));

//*************************************argument Management ******************************************
	if(argc!=5 && argc!=6){
		printf("Error\n Usage: <%s> <portNumber> <hostIP> <hostPort> <fileName> \n",argv[0]);
		printf("OR\n Usage: <%s> <-new> <portNumebr> <hostIP> <hostPort> <fileName>\n",argv[0]);
		exit(1);
	}else if(argc==5){
		position=1;
	}else if(argc==6){
		if(strcmp("-new",argv[1])==0){
			newClientFlag=0;
			printf("Seding a new Join request to client\n");
			position=2;
		}
		
	}
		//setting up client's port number 
		si_me.sin_port=(u_short)(atoi(argv[position])); //1 no position
		
		//setting up host port number
		si_other.sin_port = htons(atoi(argv[position+2])); //3 no position
		
		//setting up host IP address: 
		si_other.sin_addr.s_addr=inet_addr(argv[position+1]);    //2 no position

		printf("the port is %hu\n",si_me.sin_port); 
		
		//setting up filename
		fileName=(argv[position+3]); //4th Position  

	printf("Satisfying compiler%d %d %s\n",s,slen,buffer);
//******i**********************************************************************************************
        si_other.sin_family = AF_INET;
       //	buffer='Hello server';
	
	
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
	
	int id=1,rcv,flag=1;
	//time_t t;
	int clientID;
	
	//initializing the random number generator seed
	srand((unsigned int) time(&t));

	int subtractFactor= rand()%1571;
	clientID=abs((int) getpid()-subtractFactor);
	printf("Client ID is: %d and subtract factor is%d\n",clientID,subtractFactor);
	


	//Initializing the semaphore lock
	sem_init(&fileLock,0,1);
	sem_init(&threadLock,0,1);

	//Creating user entry thread
	pthread_create(&tid, NULL,userEntry, NULL);

	//Main thread will continue to select the peer to get winning client
	//It will also initiate the token, and passes through the clients

	while(flag){
		sendto(socketfd,&id, sizeof (&id), 0, (struct sockaddr *) &(peerInfo[0]), (socklen_t)slen);
		if(id>0){
			id++;
		}else if(id<0){
			id=0;
		}
		recvfrom(socketfd,&rcv,sizeof(&rcv),0,(struct sockaddr *)&si_other, (socklen_t*)&slen);
		printf("Received Token Number:%d\n",rcv);
		if(rcv==clientID){
			printf("This is the winning client\n Initiating token passing\n");
			id=-3;
			//break;
		}else if(rcv==0){
			id=0;
			printf("Waiting for Token\n");
		}else if(rcv<0){
			printf("I gain the token %d\n*******************\n",rcv);
			
			//Getting the semaphore locked
			sem_wait(&fileLock);
			
			//Getting the file for I/O operation
			filePointer=fopen(fileName,"a+");
				
			//Checking whether anything is there to write
			if(globalMessage[0]!='\0'){
				printf("There is something to write\n");
				printf("The message is: %s\n",globalMessage);
				//int result=fputs(globalMessage, filePointer);
				int result=fprintf(filePointer, "%s\n",globalMessage);
				//if(result==EOF){
				//	printf("Error in writing file\n");
				//}
				
			}
			
			//checking if the user wanted to read anything
			if(readFlag!=0){
				printf("User wanted to read %dth Message\n",readFlag);
			}

			//Checking if the user wanted to list everything
			if(listFlag==1){
				printf("Here is the list");
					//Need to complete the code by adding I/O operation
	
			}
			
			//checking if the peer wanted to exit
			if(exitFlag==1){
				flag=0;
			
//****************  before exiting the client I need to preserve the ring structure*********************
			//Ring preservation code goes here
					

			}
			

			//once the writing has done, reset all the values of global variable
			readFlag=0;
			listFlag=0;
			memset(globalMessage,0,sizeof(globalMessage));
			globalMessage[0]='\0';
			fclose(filePointer);
						
			sleep(4);//Slowing down the loop for observation
			id=rcv;//Which means that after sending the token I am worthless 

			//Getting the semaphore unlocked
			sem_post(&fileLock);
		}
	}


	//Memory Leak Management
        close(s);
	close(socketfd);
	pthread_join(tid,NULL);
	sem_destroy(&threadLock);
	sem_destroy(&fileLock);

	
















//*************************************     DON'T GO DOWN,WANNA GO TO HELL?     *************************************




	return 0;

}


