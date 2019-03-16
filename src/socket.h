#ifndef SOCKET_H
#define SOCKET_H

#include <thread>
#include <list>

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
    ServerNotReachable
};

enum SessionStatus
{
    Connected,
    Disconnected,
    Connecting,
    Disconnecting,
    ReadytoRead
};

class Session
{
protected:
    virtual void eventLoop()=0;
    std::thread *mEventThread=0;
public:
    bool writeBuffer(const char *aBuffer);
#ifdef __WINSOCK__
    //TODO
#else
    int mSockFd=-1;
    struct sockaddr_in mServerAddr, mClientAddr;
#endif

};

class TcpClientSessionListener
{
public:
     virtual void onError(const SessionError &aErrorCode)=0;
    virtual void dataAvailable(const char *aData)=0;
};


class TcpClientSession:public Session
{
public:
    TcpClientSession(TcpClientSessionListener *aListner);
    void setListener(TcpClientSessionListener *aListener)
    {
        mListener=aListener;
    }

    bool setSocketDescriptor(const int &aFd);
    bool connectToHost(const char *aIp,const int &aPort);
    bool disconnect();

protected:
   TcpClientSessionListener *mListener=0;
    void eventLoop();
};

class TcpServerSessionListener
{
public:
   virtual void onError(const SessionError &aErrorCode)=0;
   virtual void newConnection(const TcpClientSession *t)=0;
};

class TcpServerSession:public Session
{
public:
    TcpServerSession(TcpServerSessionListener *aListner);
    bool start(const char *aIp,const int &aPort);
    bool stop();

protected:
    void eventLoop();
    TcpServerSessionListener *mListener;
    std::list<TcpClientSession*> mActiveClients;
};

#endif // SOCKET_H
