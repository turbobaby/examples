/*
* Renan Santana / PID: 4031451 / HW3 / DUE: NOV 13, 2014
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
//
#include <errno.h>
//

const int MAX_BUFFER_SIZE = 1024;
const int FD_STDIN = 0;		/* Watch stdin (fd 0) to see when it has input. */

void syserr(char* msg) { perror(msg); exit(-1); }

void cltFormat(char buffer[]){
	int n = 0;
	char temp[170];
	char tempbuffer[1024];
	
	memset(temp, 0, sizeof(temp));
	memset(tempbuffer, 0, sizeof(tempbuffer));
	
	for(; n < 1024; n++){ if(buffer[n] == '@'){ buffer[n] = '\0'; break; } }
	
	memcpy(temp, buffer+n+2, sizeof(temp));	// cpy the user to temp
	memcpy(tempbuffer, buffer, n); 				// cpy the msg
	
	memset(buffer, 0, sizeof(tempbuffer));		// clear the buffer
	
	strcpy(buffer, temp);							// cpy the user
	strcat(buffer, ": ");
	strcat(buffer, tempbuffer);					// cat the msg
}

int srvFormat(char buffer[], int *indexOfLastUser){
	int n = 0;
	char *token;
	char temp[170];
	char tempbuffer[1024];
	
	memset(temp, 0, sizeof(temp));
	memset(tempbuffer, 0, sizeof(tempbuffer));
	
	memcpy(temp, buffer, *indexOfLastUser);		// cpy the users
	memcpy(tempbuffer, buffer+(*indexOfLastUser)+1, sizeof(tempbuffer)); // cpy the msg
	
	memset(buffer, 0, sizeof(tempbuffer));		// clear the buffer
	strcpy(buffer, tempbuffer);					// cpy the msg to buffer
	
	strcat(buffer, " @ ");							// cat @
	strcat(buffer, temp);							// cat the users
	
	return 0;
}

int nousers(char buffer[], int *indexOfLastUser){
	int n = 0, users = 1;
	*indexOfLastUser = 0;
	
	// remove the newline that is left from fgets
	for( ; n < 1024; n++){ if(buffer[n] == '\n'){ buffer[n] = '\0'; break; } }
	
	// read up to ':' which means the last of the users
	for(n = 0; n < 170; n++){
		if(buffer[n] == ',' || buffer[n] == ';' || buffer[n] == '/' || buffer[n] == '.'){ buffer[n] = ' '; }
		if(buffer[n+1] == ',' || buffer[n+1] == ';' || buffer[n+1] == '/' || buffer[n+1] == '.'){ buffer[n+1] = ' '; }
		if(n > 0 && buffer[n-1] != ' ' && buffer[n] == ':'){
			buffer[n] = ' ';
			*indexOfLastUser = n;
			return users;
		}
		else if((buffer[n] != ' ' && buffer[n+1] == ' ')){ ++users; }
	}
	return 0;
}

int main(int argc, char* argv[]){
	int srvsock, portno, n, result, indexOfLastUser, no_users, bool_quit = 0;
	struct hostent* server;
	struct sockaddr_in serv_addr;
	char leave[5], join[4], quit[4];
	char buffer[1024];
	
	/* select */
	struct timeval time;
	time.tv_sec = 1;
	time.tv_usec = 0;
	fd_set set;
	/* select */
	
	if(argc != 4) {
		fprintf(stderr, "Usage: %s <server-ip> <server-port> <client-username>\n", argv[0]);
		return 1;
	}
	
	server = gethostbyname(argv[1]);
	if (server == NULL) {
		fprintf(stderr, "ERROR: no such host: %s\n", argv[1]);
		return 2;
	}
	
	if (strcmp(argv[3], "all") == 0) {
		printf("Username (all) is a keyword please pick a different username.\n");
		return 3;
	}
	
	if (strlen(argv[3]) >= 16) {
		printf("Username %s is too long. Maximal length is 15.\n", argv[3]);
		return 3;
	}
	
	portno = atoi(argv[2]);
	
	srvsock = socket(AF_INET, SOCK_STREAM, 0);
	if (srvsock < 0) {
		syserr("can't open socket");
	}

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr   = *((struct in_addr*)server->h_addr);
	serv_addr.sin_port   = htons(portno);

	if (connect(srvsock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
		syserr("Can't connect to server");
	
	printf("Connection to %s:%s established. Now awaiting commands.\n", argv[1], argv[2]);
	
	// send to server the username we picked
	memset(buffer, 0, sizeof(buffer));
	strcpy(buffer, argv[3]);
	n = send(srvsock, buffer, MAX_BUFFER_SIZE, 0);
	if (n < 0) {
		syserr("Can't send");
	}
	
	// recv a comfirmation: name is valid
	memset(buffer, 0, sizeof(buffer));
	n = recv(srvsock, buffer, MAX_BUFFER_SIZE, 0);
	if (n < 0){
		syserr("Can't receive from client");
	}
	
	// if valid display all that are connected // else terminate
	if (buffer[0] != '\0'){
		printf("%s\n", buffer);
	} else {
		close(srvsock);
		printf(">> %s << already exist. Please choose a different one.\n", argv[3]);
		return 0;
	}
	
	printf("@ ");
	fflush(stdout);
	
	while (1) {
		FD_ZERO(&set);				// reset flags
		FD_SET(srvsock, &set);
		FD_SET(FD_STDIN, &set);
		// wait for an event
		result = select(srvsock + 1, &set, NULL, NULL, &time);
		
		// input event
		if (result > 0) {
			if (FD_ISSET(FD_STDIN, &set)){
				do {
					memset(buffer, 0, sizeof(buffer));
					while (fgets(buffer, sizeof(buffer), stdin) == NULL) {
						//printf("Enter some text\n");
					}
					no_users = nousers(buffer, &indexOfLastUser);
					memset(quit, 0, sizeof(quit));
					for (n = 0; n < 170; n++){
						if (buffer[n] == 'q'){
							memcpy(quit, buffer+n, sizeof(quit));	// get the first 4 chars "quit"?
							break;
						}
					}
					if (strcmp(quit, "quit") == 0){
						memcpy(buffer, quit, sizeof(quit));
						bool_quit = 1;
						break;
					}
					
					if (no_users == 0) {
						printf("@ Missing colon (:). after inputting user name(s).\n");
						printf("@ ");
					}
					if (no_users > 10) {
						no_users = 0;
						printf("@ Client can only send to 10 specific users.\n");
						printf("@ ");
					}
				} while(no_users <= 0);
				
				if (!bool_quit) {
					srvFormat(buffer, &indexOfLastUser);
				}
				
				// send the msg
				n = send(srvsock, buffer, MAX_BUFFER_SIZE, 0);
				if (n < 0) {
					syserr("Can't send");
				}
				
				if (bool_quit) {
					break;
				}
			}
			
			if(FD_ISSET(srvsock, &set)){
				memset(buffer, 0, sizeof(buffer));
				n = recv(srvsock, buffer, MAX_BUFFER_SIZE, 0);
				if (n <= 0) {
					break;
				}
				
				memset(leave, 0, sizeof(leave));
				memset(join, 0, sizeof(join));
				
				if(buffer[0] == 'l') {
					memcpy(leave, buffer, sizeof(leave));
				} else if(buffer[0] == 'j') {
					memcpy(join, buffer, sizeof(join));
				}
				
				if (!(strcmp(leave, "leave") == 0 || strcmp(join, "join") == 0)) {
					cltFormat(buffer);
				}
				
				printf("%s\n", buffer);	// output
			}
			printf("@ ");
			fflush(stdout);
		}
	}
	
	printf("Connection to server terminated. Bye now!\n");
	close(srvsock);
	return 0;
}
