#ifndef TCPSERVERSESSION_H
#define TCPSERVERSESSION_H

#include "socket.h"

class TcpClientSession;
class TcpServerSessionListener
{
public:
   virtual void onError(const SessionError &aErrorCode){}
   virtual void statusChanged(const SessionStatus &aStatus){}
   virtual void newConnection(TcpClientSession *t)=0;
};

class TcpServerSession:public Session
{
public:
    TcpServerSession(TcpServerSessionListener *aListner);
    ~TcpServerSession();
    bool start(const char *aIp,const int &aPort);
    bool stop();

protected:
    void eventLoop();
    void cleanupThread();
    TcpServerSessionListener *mListener;
    std::vector<TcpClientSession*> mActiveClients;
    std::thread *mCleanupThread=0;
    std::mutex mMutex;
};

#endif // TCPSERVERSESSION_H
