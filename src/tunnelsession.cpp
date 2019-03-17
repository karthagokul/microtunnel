#include "tunnelsession.h"

TunnelSession::TunnelSession(TunnelConfig *aConfig)
{
    mConfig=aConfig;
    LOG(DEBUG)<<"Cipher : "<<aConfig->name;
    LOG(DEBUG)<<"Cipher : "<<aConfig->cipher;
    LOG(DEBUG)<<"Source Host : "<<aConfig->sourcehost;
    LOG(DEBUG)<<"Source Port : "<<aConfig->sourceport;
    LOG(DEBUG)<<"Target Host : "<<aConfig->targethost;
    LOG(DEBUG)<<"Target Port : "<<aConfig->targetport;

    netlinkServer=new TcpServerSession(this);
    netlinkServer->start(mConfig->sourcehost.c_str(),mConfig->sourceport);

}

void TunnelSession::onError(const SessionError &aErrorCode)
{

}
void TunnelSession::statusChanged(const int &aSockFd, const SessionStatus &aStatus)
{

}

void TunnelSession::newConnection(TcpClientSession *t)
{
    //LOG(DEBUG)<<"Tunnel got the connection";
    ProxyConnection *s=new ProxyConnection();
    s->reciver=t;
    s->sender=new TcpClientSession(s);
    s->sender->connectToHost(mConfig->targethost.c_str(),mConfig->targetport);
    s->reciver->setListener(s);
    mConnectionPool.push_back(s);
}

TunnelSession::~TunnelSession()
{
    LOG_FUNCTION_NAME;
    if(netlinkServer)
    {
        delete netlinkServer;

    }

    std::vector<ProxyConnection*>::iterator it = mConnectionPool.begin();
    for (; it != mConnectionPool.end(); ++it) {
        delete *it;
    }
    mConnectionPool.clear();
}

void ProxyConnection::onError(const SessionError &aErrorCode)
{

}
void ProxyConnection::statusChanged(const int &aSockFd, const SessionStatus &aStatus)
{
    LOG(DEBUG)<<"Status Changed in "<<aSockFd;
    LOG(DEBUG)<<"Sender id is "<<sender->id();
    LOG(DEBUG)<<"Reciver id is"<<reciver->id();

    if(aStatus==Disconnected)
    {
        if(aSockFd==reciver->id())
        {
            sender->disconnect();
        }
        else
        {
            reciver->disconnect();
        }
    }
}

void ProxyConnection::dataAvailable(const int &aSockFd, const char *aData)
{
    LOG(DEBUG,DEBUG_LEVEL_3)<<"Data is ready on "<<aSockFd;
    LOG(DEBUG,DEBUG_LEVEL_3)<<"Sender id is "<<sender->id();
    LOG(DEBUG,DEBUG_LEVEL_3)<<"Reciver id is"<<reciver->id();

    bool ret=false;
    if(aSockFd==reciver->id())
    {
        ret=sender->writeBuffer(aData);
    }
    else
    {
        ret=reciver->writeBuffer(aData);
    }
    if(ret!=true)
    {
        LOG(ERROR)<<"Broken Net Link : Closing the session";
        //  sender->disconnect();
        reciver->disconnect();
    }
}

ProxyConnection:: ~ProxyConnection()
{
    if(reciver)
    {
        delete reciver;
        reciver=0;
    }
    if(sender)
    {
        delete sender;
        sender=0;
    }
}

