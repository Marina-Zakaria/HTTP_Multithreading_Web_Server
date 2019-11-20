#include "utiles.h"

void DieWithError(char *errorMessage)
 {
 perror (errorMessage) ;
 exit(1);
 }

int createTCPServerSocket (int serverPort){

    int sock; /* Socket to create */
    struct sockaddr_in echoServAddr; /* Local address */
    /* Create socket for incoming connections */
    if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        DieWithError(" socket () failed") ;
    /* Construct local address structure */
    memset(&echoServAddr, 0, sizeof(echoServAddr)); /* Zero out structure */
    echoServAddr.sin_family = AF_INET; /* Internet address family */
    echoServAddr.sin_addr.s_addr = htons(INADDR_ANY); /* Any incoming interface */
    //echoServAddr.sin_addr.s_addr = htons("127.0.0.1"); /* Any incoming interface */
    echoServAddr.sin_port = htons(serverPort); /* Local port */
    /* Bind to the local address */
    if (bind(sock, (struct sockaddr *)&echoServAddr, sizeof(echoServAddr)) < 0)
        DieWithError ("bind () failed");
    /* Mark the socket so it will listen for incoming connections */
    if (listen(sock, MAX_PENDING) < 0)
        DieWithError("listen() failed") ;
    return sock;

}

int AcceptTCPConnection(int serverWelcomeSocket){

    int clntSock; /* Socket descriptor for client */
    struct sockaddr_in echoClntAddr; /* Client address */
    unsigned int clntLen; /* Length of client address data structure */
    /* Set the size of the in-out parameter */
    clntLen = sizeof(echoClntAddr);
    /* Wait for a client to connect */
    if ((clntSock = accept(serverWelcomeSocket, (struct sockaddr *) &echoClntAddr,&clntLen)) < 0)
    DieWithError( "accept () failed") ;
    /* clntSock is connected to a client! */
    printf("Handling client %s\n", inet_ntoa(echoClntAddr.sin_addr)) ;
    return clntSock;


}
char * formatResponse (struct_request *currentRequest){
    char* response;
    if(currentRequest->type=='0'){
        response=malloc(MAX_REQUEST+MAX_FILE_SIZE);
        strcat(response,"HTTP/1.1 ");
        FILE* targetFile=fopen(currentRequest->filePath,"r");
        if(!targetFile){
            strcat(response,"404 NOT FOUND\r\n\r\n");
            return response;
        }
        char* source;
        int bufsize;
        if (fseek(targetFile, 0L, SEEK_END) == 0) {
        /* Get the size of the file. */
         bufsize = ftell(targetFile);
        if (bufsize == -1) { /* Error */ }

        /* Allocate our buffer to that size. */
        source = malloc(sizeof(char) * (bufsize + 1));

        /* Go back to the start of the file. */
        if (fseek(targetFile, 0L, SEEK_SET) != 0) { /* Error */ }

        /* Read the entire file into memory. */
        size_t newLen = fread(source, sizeof(char), bufsize, targetFile);
        if ( ferror( targetFile ) != 0 ) {
            fputs("Error reading file", stderr);
        } else {
            source[newLen++] = '\0'; /* Just to be safe. */
        }
    }
    fclose(targetFile);
    strcat(response,"200 OK\r\n");
    strcat(response,"Content-Length: ");
    char* length =malloc(5);
    sprintf( length, "%d",bufsize);
    strcat(response,length);
    strcat(response,"\r\n\r\n");
    strcat(response,source);
    return response;
    }
    if(currentRequest->type=='1'){
        response=malloc(MAX_REQUEST);
        strcat(response,"HTTP/1.1 ");
        FILE* targetFile=fopen(currentRequest->filePath,"wb");
        if(!targetFile){
            strcat(response,"404 NOT FOUND\r\n\r\n");
            return response;
        }

        fwrite(currentRequest->body,currentRequest->contentLength,1,targetFile);
        strcat(response,"200 OK\r\n\r\n");
        return response;
    }
}

int spliter ( char *str, char c, char ***arr){
    int count = 1;
    int token_len = 1;
    int i = 0;
    char *p;
    char *t;

    p = str;
    while (*p != '\0')
    {
        if (*p == c)
            count++;
        p++;
    }

    *arr = (char**) malloc(sizeof(char*) * count);
    if (*arr == NULL)
        exit(1);

    p = str;
    while (*p != '\0')
    {
        if (*p == c)
        {
            (*arr)[i] = (char*) malloc( sizeof(char) * token_len );
            if ((*arr)[i] == NULL)
                exit(1);

            token_len = 0;
            i++;
        }
        p++;
        token_len++;
    }
    (*arr)[i] = (char*) malloc( sizeof(char) * token_len );
    if ((*arr)[i] == NULL)
        exit(1);

    i = 0;
    p = str;
    t = ((*arr)[i]);
    while (*p != '\0')
    {
        if (*p != c && *p != '\0')
        {
            *t = *p;
            t++;
        }
        else
        {
            *t = '\0';
            i++;
            t = ((*arr)[i]);
        }
        p++;
    }

    return count;
}
