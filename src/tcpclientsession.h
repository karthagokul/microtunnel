#ifndef TCPCLIENTSESSION_H
#define TCPCLIENTSESSION_H

#include "socket.h"

class TcpClientSessionListener
{
public:
    virtual void onError(const SessionError &aErrorCode){}
    virtual void statusChanged(const SessionStatus &aStatus){}
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
    SessionStatus status() const
    {
        return mStatus;
    }
    bool setSocketDescriptor(const int &aFd);
    bool connectToHost(const char *aIp,const int &aPort);
    bool disconnect();
    ~TcpClientSession();
    std::mutex mMutex;

protected:
   TcpClientSessionListener *mListener=0;
    void eventLoop();
   SessionStatus mStatus=Disconnected;
};

#endif // TCPCLIENTSESSION_H
