#ifndef TUNNELSESSION_H
#define TUNNELSESSION_H

#include "tcpclientsession.h"
#include "tcpserversession.h"
#include "configurator.h"

class ProxyConnection:public TcpClientSessionListener
{
public:
    ~ProxyConnection();
    TcpClientSession *reciver=0,*sender=0;
    void onError(const SessionError &aErrorCode);
    void statusChanged(const int &aSockFd, const SessionStatus &aStatus);
    void dataAvailable(const int &aSockFd, const char *aData);
};

class TunnelSession:public TcpServerSessionListener
{
public:
    TunnelSession(TunnelConfig *aConfig);
    TcpServerSession* netlinkServer;
    std::vector<ProxyConnection*> mConnectionPool;
    TunnelConfig *mConfig; //Should it be a pointer ?
    void newConnection(TcpClientSession *t);
    void onError(const SessionError &aErrorCode);
    void statusChanged(const int &aSockFd, const SessionStatus &aStatus);
    ~TunnelSession();
};


#endif // TUNNELSESSION_H
