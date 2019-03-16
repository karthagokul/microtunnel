#include "socket.h"
#include "logger.h"
#include <iostream>

TcpClientSession::TcpClientSession(TcpClientSessionListener *aListener)
    :mListener(aListener)
{
    LOG(DEBUG) << "Main executed with ";;
}

void TcpClientSession::eventLoop()
{
    LOG_FUNCTION_NAME;
    //Mutex the mSockFd
    char buff[SOCKET_BUFFER_SIZE];
    for (;;) {
        bzero(buff, sizeof(buff));
        read(mSockFd, buff, sizeof(buff));
        if(mListener)
        {
            mListener->dataAvailable(buff);
        }
        else
        {
            LOG(DEBUG)<<"Client listener is not available yet";
        }
        LOG(DEBUG)<<buff;
    }
}

bool Session::writeBuffer(const char *aBuffer)
{
    LOG_FUNCTION_NAME;
    if(write(mSockFd, aBuffer, sizeof(aBuffer))<=0)
    {
        LOG(ERROR)<<"Sending Buffer failed";
        return false;
    }
    return true;
}

bool TcpClientSession::connectToHost(const char *aIp,const int &aPort)
{
    mSockFd = socket(AF_INET, SOCK_STREAM, 0);
    if (mSockFd == -1) {
        LOG(ERROR)<<"Invalid Socket";
        return false;
    }

    memset(&mServerAddr, 0,sizeof(mServerAddr));

    mServerAddr.sin_family = AF_INET;
    mServerAddr.sin_addr.s_addr = inet_addr(aIp);
    mServerAddr.sin_port = htons(aPort);
    if (connect(mSockFd, (struct  sockaddr*)&mServerAddr, sizeof(mServerAddr)) != 0)
    {
        LOG(ERROR)<<"Unable to Connect";
        return false;
    }
    mEventThread=new std::thread(&TcpClientSession::eventLoop,this);
    return true;
}

bool TcpClientSession::setSocketDescriptor(const int &aFd)
{
    LOG_FUNCTION_NAME;
    mSockFd=aFd;
    mEventThread=new std::thread(&TcpClientSession::eventLoop,this);
}

bool TcpClientSession::disconnect()
{
    //change
    if(mSockFd==-1)
    {
        LOG(ERROR)<<"Disconnect Failed, Invalid Socket";
        return false;
    }
    if(!mEventThread)
    {
        LOG(ERROR)<<"Disconnect Failed, Event loop was empty";
        return false;
    }

    mEventThread->join();
    close(mSockFd);
    return true;
}

TcpServerSession::TcpServerSession(TcpServerSessionListener *aListner)
    :mListener(aListner)
{
    LOG_FUNCTION_NAME;
}

bool TcpServerSession::start(const char *aIp,const int &aPort)
{
    LOG_FUNCTION_NAME;
    mSockFd = socket(AF_INET, SOCK_STREAM, 0);
    if (mSockFd == -1) {
        LOG(ERROR)<<"Invalid Socket";
        return false;
    }

    memset(&mServerAddr, 0,sizeof(mServerAddr));

    mServerAddr.sin_family = AF_INET;
    mServerAddr.sin_addr.s_addr = inet_addr(aIp);
    mServerAddr.sin_port = htons(aPort);

    if ((bind(mSockFd,(struct  sockaddr*)&mServerAddr, sizeof(mServerAddr))) != 0)
    {
        LOG(ERROR)<<"Bind Failed";
        return false;
    }


    mEventThread=new std::thread(&TcpServerSession::eventLoop,this);
    return true;
}


bool TcpServerSession::stop()
{
    return true;
}

void TcpServerSession::eventLoop()
{
    LOG_FUNCTION_NAME;
    while(1)
    {
        // Now server is ready to listen and verification
        if ((listen(mSockFd, 5)) != 0) {
            LOG(ERROR)<<"Listen Failed";
            exit(0);
        }

        int len = sizeof(mClientAddr);
        // Accept the data packet from client and verification
        int connfd = accept(mSockFd, (struct  sockaddr*)&mClientAddr, (socklen_t*)&len);
        if (connfd < 0) {
            LOG(ERROR)<<"Accept Failed";
            exit(0);
        }
        TcpClientSession *s=new TcpClientSession(0);
        s->setSocketDescriptor(connfd);
    }
}
