#ifndef RECIEVER_H_INCLUDED
#define RECIEVER_H_INCLUDED

#include "utiles.h"

struct_request* parseRequest(int socket,char *httpRequest,int status);
int getContentParam(int *contentStart,char *httpRequest);

#endif // RECIEVER_H_INCLUDED
