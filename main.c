#include "main.h"

void *threadMain(void *threadArgs) ;

int main(int argc,char *argv[])
{
    int clientSocket;
    int serverWelcomeSocket;
    unsigned short serverPort;
    pthread_t threadID;
    struct_threadArgs *threadArgs;

    serverPort = atoi(argv[1]); /* First arg' local port */
    serverWelcomeSocket = createTCPServerSocket (serverPort) ;
    while(1) /* Run forever */
    {
    clientSocket = AcceptTCPConnection(serverWelcomeSocket) ;
    /* Create separate memory for client argument */
    if ((threadArgs = (struct_threadArgs *) malloc(sizeof(struct_threadArgs)))== NULL)
        DieWithError("malloc() for thread args failed");
    threadArgs -> clientSocket = clientSocket;
    /* Create client thread */
    if (pthread_create(&threadID, NULL, threadMain, (void *) threadArgs) != 0)
        DieWithError("pthread_create() failed");
    printf("with thread %d\n", (long int) threadID);
}
    printf("Hello world!\n");

    return 0;
}

void *threadMain(void *threadArgs) {

    int clientSocket;
    /* Guarantees that thread resources are deallocated upon return */
    pthread_detach(pthread_self() ) ;
    /* Extract socket file descriptor from argument */
    clientSocket = ((struct_threadArgs *) threadArgs) -> clientSocket;
    free(threadArgs); /* Deallocate memory for argument */
    printf("from inside thread %d\n",clientSocket);
    //handle client///////////////////////////
    char* receiveBuffer =malloc(MAX_REQUEST+MAX_FILE_SIZE);
    while(1){
    int status = recv(clientSocket,receiveBuffer,MAX_REQUEST,0);
    printf("%s",receiveBuffer);
    if(status<=0){
        perror("error receiving packages");
        exit(0);
    }
    struct_request* currentRequest = malloc(sizeof(struct_request));
    currentRequest=parseRequest(clientSocket,receiveBuffer,status);

    char* sendBuffer=malloc(MAX_REQUEST+MAX_FILE_SIZE);
    sendBuffer=formatResponse( currentRequest);

    send(clientSocket , sendBuffer , strlen(sendBuffer) , 0 );
    printf("\n we made it here response is %s",sendBuffer);
    }
    //close(clientSocket);
}
