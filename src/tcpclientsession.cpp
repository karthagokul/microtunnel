#include "tcpclientsession.h"

TcpClientSession::TcpClientSession(TcpClientSessionListener *aListener)
    :mListener(aListener)
{
}

void TcpClientSession::eventLoop()
{
    LOG_FUNCTION_NAME;

    mMutex.lock();
    mStatus=Connected;
    mMutex.unlock();

    char buff[SOCKET_BUFFER_SIZE];
    for (;;) {
        bzero(buff, sizeof(buff));
        int readSize=read(mSockFd, buff, sizeof(buff));
        if(readSize<=0)
        {
            mMutex.lock();
            if(getError()!=NoError)
            {
                LOG(ERROR)<<"Failed to read";
            }
            else
            {
                LOG(DEBUG)<<"Client has been disconnected";
            }
            mMutex.unlock();
            break;
        }
        mMutex.lock();
        if(mListener)
        {
            mListener->dataAvailable(buff);
        }
        mMutex.unlock();
    }
    disconnect();
}

TcpClientSession::~TcpClientSession()
{
    LOG_FUNCTION_NAME;
    disconnect();
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
    mMutex.lock();
    //change
    if(mSockFd>=0)
    {
        LOG(ERROR)<<"Closing the Socket";
        close(mSockFd);
        mSockFd=-1;
    }
    mStatus=Disconnected;
    mMutex.unlock();
    return true;
}
