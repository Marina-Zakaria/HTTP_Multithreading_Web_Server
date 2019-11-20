#include "reciever.h"


struct_request* parseRequest(int socket,char *httpRequest,int status){
    struct_request* parsedRequest = malloc (sizeof(struct_request));
    //parse the request to extract type and file path
    int i=0;
    char* line1=malloc(MAX_CHAR);
    //get first line of response
    while(httpRequest[i]!='\r'&&httpRequest[i]!='\n'){
        line1[i]=httpRequest[i];
        i++;
    }
    i=0;
    char* requestType = malloc(5);
    //get cursor i to space after request type
    while(line1[i]!=' '){
        requestType[i]=line1[i];
        i++;
    }
    requestType[i]='\0';
    //get type of request
    if(strcmp(requestType,"GET")==0){
        parsedRequest->type='0';
    }else if(strcmp(requestType,"POST")==0){
        parsedRequest->type='1';}
    while(line1[i]==' '||line1[i]=='/'){
        i++;
    }
    char* filePath = malloc(MAX_CHAR);
    int j=0;
    //get cursor i to space after request type
    while(line1[i]!=' '){
        filePath[j++]=line1[i];
        i++;
    }
    filePath[j]='\0';
    //get type of request
    parsedRequest->filePath=filePath;

    char **arr = malloc(MAX_CHAR);
    int  c = spliter(filePath,'/',&arr);
    parsedRequest->fileName=arr[c-1];
    if(parsedRequest->type=='0'){
        return parsedRequest;
    }
    int contentStart ;
    int contentLength = getContentParam(&contentStart,httpRequest);
    parsedRequest->contentStart=contentStart;
    parsedRequest->contentLength=contentLength;

    if(contentLength <= 0){
        perror("Invalid POST REQUEST As no content exist in the request");
        exit(0);
        return;
    }
    //int status = recv(socket, httpRequest, contentStart + contentLength, MSG_PEEK);
     char * content=malloc(contentLength+1);
     int current_index=0;
      // we may not receive all the data content completely if the data content is large.
    for(; current_index < status - contentStart; current_index++){
        content[current_index] = httpRequest[current_index + contentStart];
    }
    // continue reading until we finish reading all the content
    while(contentLength - current_index > 1){
        status = recv(socket, (char *)(content + current_index), contentLength - current_index, 0);
        current_index += status;
    }
    parsedRequest->body=content;

    return parsedRequest;
}
int getContentParam(int *contentStart,char *httpRequest){
char *dest = strstr(httpRequest, "Content-Length");
int pos= dest - httpRequest;
while(httpRequest[pos]<'0'||httpRequest[pos]>'9'){
    pos++;
}
char* contentLength = malloc(10);
int j=0;
while(httpRequest[pos]>='0'&&httpRequest[pos]<='9'){
        contentLength[j++]=httpRequest[pos++];
    }
char *dest1 = strstr(httpRequest, "\r\n\r\n");
pos= dest1 - httpRequest+4;
*contentStart=pos;
return atoi (contentLength);

}
