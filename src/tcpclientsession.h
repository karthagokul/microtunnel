#ifndef TCPCLIENTSESSION_H
#define TCPCLIENTSESSION_H

#include "socket.h"

class TcpClientSessionListener:public SessionListener
{
public:
    virtual void dataAvailable(const int &aSockFd, const char *aData)=0;
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
    ~TcpClientSession();


protected:
    TcpClientSessionListener *mListener=0;
    void eventLoop();
};

#endif // TCPCLIENTSESSION_H
