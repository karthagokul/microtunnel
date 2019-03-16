#ifndef SOCKET_H
#define SOCKET_H

#include <thread>
#include <mutex>
#include <vector>
#include "logger.h"

#ifdef __WINSOCK__
    //TODO
#else
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif

#define SOCKET_BUFFER_SIZE 1024

enum SessionError
{
    Unknown=0,
    HostNotFound,
    ServerNotReachable,
    NoError
};

enum SessionStatus
{
    Connected,
    Disconnected,
    Connecting,
    Disconnecting,
    ReadytoRead
};

//dirty inheritance , to be removed

class Session
{
protected:
    virtual void eventLoop()=0;
    std::thread *mEventThread=0;
public:
    bool writeBuffer(const char *aBuffer);
    SessionError getError();
#ifdef __WINSOCK__
    //TODO
#else
    int mSockFd=-1;
    struct sockaddr_in mServerAddr, mClientAddr;
#endif

};

/*
 * TODO: The dirty Socket hierarchy and link between and server and client , May be soon once i have a base workflow ready
 */

#endif

