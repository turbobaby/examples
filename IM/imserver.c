#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>

void syserr(char *msg) { perror(msg); exit(-1); }

/*
 *	Server receives format from discription.
 * ie. "msg @ user-names"
 * This method splits from the '@' into allUsers and buffer
 */
void splitBuffer(char *buffer, char *allUsers){
	int n = 1;
	for( ; n < 1024; n++){ if(buffer[n] == '@'){ break; } }
	memcpy(allUsers, buffer+n+1, 170);
	memset(buffer+n, 0, 1024);
}

/*
 *	When a user leaves we need to clean its spot
 * ie. We hash to the user's position and clean the memory
 */
void cleanBoth(char userNameTable[][16], int users[], char userName[], int socket){
	unsigned int n, val = 0, size = strlen(userName);
	strcpy(userName, userNameTable[(7*socket + 11) % 199]);
	memset(userNameTable[(7*socket + 11) % 199], 0, 16);
	
	for(n = 0; n < size; n++){ val += (unsigned char)userName[n] * (n+1); }
	users[(7*val + 11) % 199] = -1;
}

/*
 * Note: strtok first needs a pointer, say a string after that
 * 	   we just call it with NULL. If strtok returns NULL, it read
 *			the last token in the string.
 * This method returns 1 if we have another user-name.
 * Username will contain the next user-name in the string
 */
int getUserReq(char userName[]){
	char* token = strtok(NULL, " ");
	bzero(userName, 0);
	if(token != NULL){ strcpy(userName, token); return 1; }
	return 0;
}

/*
 *	This method first finds out how many users are connected.
 * Then it grabs all the user-names in the table and puts it in the buffer.
 */
void getUsersConnected(char userNameTable[][16], char buffer[]){
	bzero(buffer, 0);
	char temp[8];
	int iterate = 0, usersConnected = 0, tempCounter = 0;
	
	for( ;iterate < 199; iterate++){
		if(userNameTable[iterate][0] != 0){ ++usersConnected; }
	}
	
	sprintf(temp, "%d", usersConnected);
	strcpy(buffer, "NUMBER OF USERS CONNECTED : ");
	strcat(buffer, temp);
	strcat(buffer, " \n@ ONLINE : ");
	
	for(iterate = 0 ;iterate < 199; iterate++){
		if(userNameTable[iterate][0] != 0){
			strcat(buffer, userNameTable[iterate]);
			if(++tempCounter != usersConnected){ strcat(buffer, ", "); }
		}
	}
	strcat(buffer, "\0");
}

/*
 *	This is the main hash method to store the sockets of
 * the users connecting.
 * This method also takes care of duplicate names coming in
 * and if we hashed to a position that is already occupied.
 * This method advises if the new comer is alowed to join.
 */
int setPeerSock(int users[], char userName[], int peersock){
	unsigned int n, val = 0, i, size = strlen(userName);
	for(n = 0; n < size; n++){ val += (unsigned char)userName[n] * (n+1); }
	i = (7*val + 11) % 199;
	
	if(users[i] != -1){ return 0; }
	users[i] = peersock;
	
	return 1;
}

/*
 * If we have set the socket in the hash table then we are
 * able to enter the name in this hash table of names.
 */
int setName(char userNameTable[][16], int peersock, char userName[]){
	int i = (7*peersock + 11) % 199;
	if( userNameTable[i][0] != 0){ return 0; }
	strcpy(userNameTable[i], userName);
	return 1;
}

/*
 * This method is used to get a associated socket from the user-name
 */
int getPeerSock(int users[], char userName[]){
	unsigned int n, val = 0, size = strlen(userName);
	for(n = 0; n < size; n++){ val += (unsigned char)userName[n] * (n+1); }
	return users[(7*val + 11) % 199];
}

/*
 * This method is used to get a associated user-name from the socket
 */
char* getName(char userNameTable[][16], char userName[], int peersock){
	bzero(userName, 0);
	strcpy(userName, userNameTable[(7*peersock + 11) % 199]);
	return userName;
}

int main(int argc, char *argv[]){
	const int MAX_BUFFER_SIZE = 1024;
	char buffer[1024];
	char tempBuf[1024];
	char distributeMSG[1024];
	
	fd_set readset, tempset;
	int maxfd, portno;
	int srvsock, peersock, peerReq, result, result1, tempPeerReq;
	struct sockaddr_in serv_addr, clt_addr;
	socklen_t addrlen;
	
	
	/*Send + Recv*/
	int sendSize, j, n, sendAll = 0;
	/*Send + Recv*/
	
	/*Handle request: parse str*/
	char delimeter[] = " :";
	char allUsers[170];
	char cpy[170];
	char *token;
	/*Handle request: parse str*/
	
	/*Users table (Hash table)*/
	const int MAX_USERS = 199;
	char userName[16], senderName[16];
	char tempPlaceHolder[20];
	int users[199] = { [0 ... 198] = -1};
	char userNameTable[MAX_USERS][16];
	memset(userNameTable, 0, sizeof(char)*MAX_USERS*16);
	/*Users table*/
	
	if(argc != 2) {
		if(argc == 1){
			portno = 5555;
		}
		else{
			fprintf(stderr,"Usage: %s <port#>\n", argv[0]);
			return 1;
		}
	}
	else
		portno = atoi(argv[1]);

	srvsock = socket(AF_INET, SOCK_STREAM, 0);
	if (srvsock < 0){
        syserr("Err 0: Can't Open Socket");
    }
	printf("Create server socket.\n");

    int err, sock_reuse = 1;
    err = setsockopt(srvsock, SOL_SOCKET, SO_REUSEADDR,
                     (char *)&sock_reuse, sizeof(sock_reuse));
    if (err != 0){
        syserr("Err 0: Can't reuse Socket port");
    }

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);

	if(bind(srvsock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
		syserr("Err 0: Can't Bind To Port.");
	printf("Bind server socket to port#: %d.\n", portno);

	listen(srvsock, 5);

	FD_ZERO(&readset);
	FD_SET(srvsock, &readset);
	maxfd = srvsock;

	do {
		// tempset will be changing as readset gets new users / readset has its flags unset
		memcpy(&tempset, &readset, sizeof(tempset));
		result = select(maxfd + 1, &tempset, 0, 0, 0);
		
		// ignore result == 0 (we dont need a timeout condition)
		
		if (result < 0 && errno != EINTR) { printf("Error in select(): %s\n", strerror(errno)); }
		else if (result > 0) {
			// if the srvsock is set that means we have a incoming connection
		   if (FD_ISSET(srvsock, &tempset)) {
		   	
		   	/*** NEW CONNECTION ***/
		      addrlen = sizeof(clt_addr);
		      peersock = accept(srvsock, (struct sockaddr*)&clt_addr, &addrlen);
		      /*** NEW CONNECTION ***/

		      if (peersock < 0) { printf("Error in accept(): %s\n", strerror(errno)); }
		      else {
		         bzero(buffer, 0);
		         bzero(userName,0);

		         recv(peersock, buffer, MAX_BUFFER_SIZE, 0);	// recv name
		         sscanf(buffer, "%s", userName);					// parse name

		         // try put socket in table
		         int insertSock = setPeerSock(users, userName, peersock);

		         if(!insertSock){			// socket was not inserted
			         printf("DUPLICATE USER: %s\n", userName);
		         	bzero(buffer, 0);
						strcpy(buffer, "\0");
						send(peersock, buffer, MAX_BUFFER_SIZE, 0);
				      close(peersock);
					}
					else{							// socket was inserted
						printf("NEW USER: %s\n", userName);
						setName(userNameTable, peersock, userName);	// put name in a table

				      /* send all that are connected to the new arrival*/
				      getUsersConnected(userNameTable, tempBuf);
				      strcpy(buffer, "@ ");
				      strcat(buffer, tempBuf);
				      /* send all that are connected to the new arrival*/

				      n = send(peersock, buffer, MAX_BUFFER_SIZE, 0);
				      if(n < 0){ printf("Can't send to client\n"); }

				      FD_SET(peersock, &readset);
			         maxfd = (maxfd < peersock) ? peersock : maxfd;

			         /* send all that are connected to everyone else*/
			         memset(buffer, 0, sizeof(buffer));
						strcpy(buffer, "join  > ");
						getName(userNameTable, userName, peersock);
						strcat(buffer, userName);
						strcat(buffer, "\n@ ");
						getUsersConnected(userNameTable, tempBuf);
						strcat(buffer, tempBuf);
						
						for (tempPeerReq = 0; tempPeerReq < MAX_USERS; tempPeerReq++) {
							if(users[tempPeerReq] == -1 || users[tempPeerReq] == peersock){ continue; }
							send(users[tempPeerReq], buffer, MAX_BUFFER_SIZE, 0);
						}
						/* send all that are connected to everyone else*/
					}
		      }
		   }	// end of new connection
			
			// we remove the srvsock from the tempset so that we dont try to read itself
			FD_CLR(srvsock, &tempset);
			
			/*
			*	We check if any of the accepted sockets has the FD set.
			*	PeerReq is the person that is making contact to the host.
			*	TempPeerReq is the person that we are sending the msg to.
			*/
		   for (peerReq = 0; peerReq < maxfd+1; peerReq++) {
		      if (FD_ISSET(peerReq, &tempset)) {
					memset(buffer, 0, sizeof(buffer));
					
		         do{
		            result = recv(peerReq, buffer, MAX_BUFFER_SIZE, 0);
		         }while (result == -1 && errno == EINTR);
					printf("\nINCOMING MSG: %s\n", buffer);
					
					// we are going to terminate this user
					if(strcmp(buffer, "^C") == 0 || strcmp(buffer, "^Z") == 0 || strcmp(buffer, "quit") == 0)
						{ result = 0; }
					
		         if(result > 0) {
						splitBuffer(buffer, allUsers); // split the msg from users
						
						/* get first user name in the request */
						strcpy(cpy, allUsers);				// copy the buffer
						token = strtok(cpy, delimeter);	// get first user or "all"
						bzero(userName, 0);
						strcpy(userName, token);
						userName[sizeof(userName)-1] = '\0';
						/* get first user name in the request */
						
						/* get name of the sender. */
						bzero(distributeMSG, 0);
						getName(userNameTable, senderName, peerReq);
						strcpy(tempPlaceHolder, "@ ");
						strcat(tempPlaceHolder, senderName);
						strcpy(distributeMSG, buffer);
						strcat(distributeMSG, tempPlaceHolder);
						/* get name of the sender. */
						
						if(strcmp(userName, "all") == 0){ sendAll = 1; }
						sendSize = sendAll ? MAX_USERS : 1;
						
						// loop the users
						do{
							// if specific user: get the associated socket
							if(!sendAll){
								tempPeerReq = getPeerSock(users, userName);
								if(tempPeerReq == peerReq){ continue; }
							}
							
							// send size can either be all or 1
							for(j = 0; j < sendSize; j++){
								if(sendAll){
									if(users[j] == -1){ continue; }
									tempPeerReq = users[j];
									if(tempPeerReq == peerReq){ continue; }
								}
								
								// if we try to send to a specific user and he is not there prompt the user
								if(sendAll == 0 && tempPeerReq == -1){
									bzero(tempBuf, 0);
									strcpy(tempBuf, "User is not connected @ ");
									strcat(tempBuf, userName);
									result1 = send(peerReq, tempBuf, MAX_BUFFER_SIZE, 0);
									printf("USER IS NOT CONNECTED: %s\n", userName);
								}
								else{
									printf("FROM: %s / TO: %s / SEND MSG: %s\n", senderName, userName, distributeMSG);
									result1 = send(tempPeerReq, distributeMSG, MAX_BUFFER_SIZE, 0);
								}
								
								// for what ever reason we cant reach the client we
								// need to update : not connected
						      if (result1 == 0){
						      	memset(buffer, 0, sizeof(buffer));
									strcpy(buffer, "leave: ");
									getName(userNameTable, userName, peerReq);
									strcat(buffer, userName);
						
									cleanBoth(userNameTable, users, userName, peerReq);
						
									for (tempPeerReq = 0; tempPeerReq < MAX_USERS; tempPeerReq++) {
										if(users[tempPeerReq] == -1){ continue; }
										send(users[tempPeerReq], buffer, MAX_BUFFER_SIZE, 0);
									}
									
									printf("ODD RESULT > USER IS NOT CONNECTED: %s\n", userName);
									close(tempPeerReq);
									FD_CLR(tempPeerReq, &readset);
						      }
							}
							
							// get next user
						}while(!sendAll && getUserReq(userName));
						sendAll = 0;
			      }
			      else if (result == 0){
			      	// send all that are connected
			      	memset(buffer, 0, sizeof(buffer));
						strcpy(buffer, "leave  > ");
						getName(userNameTable, userName, peerReq);
						strcat(buffer, userName);
						strcat(buffer, "\n@ ");
						
						cleanBoth(userNameTable, users, userName, peerReq);
						
						getUsersConnected(userNameTable, tempBuf);
						strcat(buffer, tempBuf);
						
						for (tempPeerReq = 0; tempPeerReq < MAX_USERS; tempPeerReq++) {
							if(users[tempPeerReq] == -1){ continue; }
							send(users[tempPeerReq], buffer, MAX_BUFFER_SIZE, 0);
						}
						
						printf("LEAVE: %s\n", userName);
			         close(peerReq);
			         FD_CLR(peerReq, &readset);
			      }
		         else { printf("Error in recv(): %s\n", strerror(errno)); }
		      }      // end if (FD_ISSET(peerReq, &tempset))
		   }      // end for (peerReq=0;...)
		}      // end else if (result > 0)
	} while (1);
	
	close(srvsock);
	return 0;
}
