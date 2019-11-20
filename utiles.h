#ifndef UTILES_H_INCLUDED
#define UTILES_H_INCLUDED
#include <stdio.h> /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), connect(), send(), and recv() */
 #include <arpa/inet.h> /* for sockaddr_in and inet_addr() */
 #include <stdlib.h> /* for atoi() */
 #include <string.h> /* for memset() */
 #include <unistd.h> /* for close() */
 #include <pthread.h> /*poxix thread*/

 #define MAX_PENDING 5
 #define MAX_REQUEST 400
 #define MAX_FILE_SIZE 1000
 #define MAX_CHAR 100
 typedef struct request{
 char type;
 char *filePath;
 char *fileName;
 char *body;
 int  contentLength;
 int  contentStart;}struct_request;
int createTCPServerSocket (int serverPort);
void DieWithError(char *errorMessage);
int AcceptTCPConnection(int serverWelcomeSocket);
int spliter ( char *str, char c, char ***arr);
char * formatResponse (struct_request *currentRequest);


#endif // UTILES_H_INCLUDED
