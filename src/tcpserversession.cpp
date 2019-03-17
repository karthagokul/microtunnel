#include "tcpserversession.h"
#include "tcpclientsession.h"


TcpServerSession::TcpServerSession(TcpServerSessionListener *aListner)
    :Session(aListner),mListener(aListner)
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

    // To avoid binding problems
    int val = 1;
    setsockopt(mSockFd,SOL_SOCKET,SO_REUSEADDR,&val,sizeof(int));

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
    LOG_FUNCTION_NAME;
    stop();
}

bool TcpServerSession::stop()
{
    LOG_FUNCTION_NAME;
    mMutex.lock();
    if(mSockFd>=0)
    {
        //Look at the client shutdown calls
        close(mSockFd);
        mSockFd=-1;
    }
    setStatus(Disconnected);
    if(mCleanupThread)
    {
        mCleanupThread->detach();
        delete mCleanupThread;
        mCleanupThread=0;
    }


    if(mEventThread)
    {
        mEventThread->detach();
        delete mEventThread;
        mEventThread=0;
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
        setStatus(Listening);
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
        LOG(DEBUG)<<"Number of Total Clients"<<mActiveClients.size();
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
