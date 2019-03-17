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

#define SOCKET_BUFFER_SIZE 32768

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
    ReadytoRead,
    Listening
};

//dirty inheritance , to be removed

class SessionListener
{
public:
    virtual void onError(const SessionError &aErrorCode)=0;
    virtual void statusChanged(const int &aSockFd, const SessionStatus &aStatus)=0;
};

class Session
{
protected:
    virtual void eventLoop()=0;
    std::thread *mEventThread=0;
public:
    Session(SessionListener *aListener):mListener(aListener)
    {

    }
    int id() const
    {
        return mSockFd;
    }

    SessionStatus status() const
    {
        return mStatus;
    }

    bool writeBuffer(const char *aBuffer);
    SessionError getError();
    bool cleanup();
#ifdef __WINSOCK__
    //TODO
#else
    int mSockFd=-1;
    struct sockaddr_in mServerAddr, mClientAddr;
    SessionListener *mListener=0; //Dirty object hierarchy TODO : Fix it dear ;)
    SessionStatus mStatus=Disconnected;
     std::mutex mMutex;
#endif

protected:
    void setStatus(SessionStatus aState)
    {
        mStatus=aState;
        if(mListener)
            mListener->statusChanged(mSockFd,mStatus);

    }

};

/*
 * TODO: The dirty Socket hierarchy and link between and server and client , May be soon once i have a base workflow ready
 */

#endif

