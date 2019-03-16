#include "socket.h"
#include "logger.h"
#include <iostream>

SessionError Session::getError()
{
    SessionError errorCode=Unknown;
    if(mSockFd!=-1)
    {
        int error = 0;
        socklen_t len = sizeof (error);
        int retval = getsockopt (mSockFd, SOL_SOCKET, SO_ERROR, &error, &len);
        if (retval != 0)
        {
            /* there was a problem getting the error code */
            fprintf(stderr, "error getting socket error code: %s\n", strerror(retval));
            errorCode=Unknown;
        }
        else
        {
            if(error!=0)
            {
             //Check error value https://www.xinotes.net/notes/note/1793/
                LOG(ERROR)<<"Socket Error Code "<<error<<": "<<strerror(error);
                errorCode=ServerNotReachable; //Temporary , This need to be fine tuned
            }
            else
            {
                errorCode=NoError;
            }
        }

    }
    return errorCode;
}

TcpClientSession::TcpClientSession(TcpClientSessionListener *aListener)
    :mListener(aListener)
{
    LOG(DEBUG) << "Main executed with ";;
}

void TcpClientSession::eventLoop()
{
    mMutex.lock();
    mStatus=Connected;
    mMutex.unlock();
    LOG_FUNCTION_NAME;

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
        else
        {
            LOG(DEBUG)<<"Client listener is not available yet";
        }
        LOG(DEBUG)<<buff;
        mMutex.unlock();
    }
    LOG(DEBUG)<<"Came out of the Event loop";

    disconnect();
}

TcpClientSession::~TcpClientSession()
{
    LOG_FUNCTION_NAME;
    disconnect();
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

    mCleanupThread=new std::thread(&TcpServerSession::cleanupThread,this);
    mEventThread=new std::thread(&TcpServerSession::eventLoop,this);

    return true;
}

TcpServerSession::~TcpServerSession()
{
    stop();
    if(mCleanupThread)
    {
        delete mCleanupThread;
        mCleanupThread=0;
    }
    if(mEventThread)
    {
        delete mEventThread;
        mEventThread=0;
    }
}

bool TcpServerSession::stop()
{
    mMutex.lock();
    if(mSockFd>=0)
    {
        close(mSockFd);
        mSockFd=-1;
    }
    mMutex.unlock();
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
        mActiveClients.push_back(s);
        if(mListener)
            mListener->newConnection(s);
    }
}

void TcpServerSession::cleanupThread()
{
    LOG_FUNCTION_NAME;
    while(1)
    {
        LOG(DEBUG)<<"Cleanup Routine : 5 seconds delay";
        std::this_thread::sleep_for(std::chrono::seconds( 5) );
        //Let's iterate through the sessions and see if any of them has been disconnected, if so , free up.
        for (std::vector<TcpClientSession *>::iterator i=mActiveClients.begin(); i!=mActiveClients.end(); i++)
        {
            TcpClientSession *t=*i;
            if(t && t->status()==Disconnected)
            {
                LOG(DEBUG)<<"Cleaning up an inactive Session";
                i=mActiveClients.erase(i);
                delete t;
                i--;
            }
        }
    }
}
