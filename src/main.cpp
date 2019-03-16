#include <iostream>
#include <signal.h>
#include "tcpclientsession.h"
#include "tcpserversession.h"
#include "logger.h"

#include "microtunnelapp.h"

using namespace std;

void sig_handler(int signo)
{
    if (signo == SIGUSR1)
        printf("received SIGUSR1\n");
    else if (signo == SIGKILL)
        printf("received SIGKILL\n");
    else if (signo == SIGSTOP)
        printf("received SIGSTOP\n");

    exit(0);
}

class TestProxy:public TcpServerSessionListener
{
public:
    void onError(const SessionError &aErrorCode)
    {
    }

    void newConnection(TcpClientSession *a)
    {
        LOG(INFO)<<"New Connection";
        TcpClientSession *s=a;
        const char *data="hello";
        s->writeBuffer(data);
    }

    TestProxy()
    {
        s=new TcpServerSession(this);
    }

    void start()
    {
        const char *aIp="127.0.0.1";
        const int port=8080;
        if(!s->start(aIp,port))
        {
            std::cerr<<"failed";
            exit(0);
        }
    }

private:
    TcpServerSession *s;
};

structlog LOGCFG = {};

int main()
{
    MicroTunnelApp app;
    app.start();

    return 0;

    //TODO : Better way
    if (signal(SIGINT, sig_handler) == SIG_ERR)
        LOG(WARN)<<"Failed:SIGINT Subscription";
    if (signal(SIGTERM, sig_handler) == SIG_ERR)
        LOG(WARN)<<"Failed:SIGTERM Subscription";
    if (signal(SIGABRT, sig_handler) == SIG_ERR)
        LOG(WARN)<<"Failed:SIGABRT Subscription";

    //Basic Logging
    LOGCFG.headers = false;
    LOGCFG.level = DEBUG;

    TestProxy t;
    t.start();

    //Main event loop
    while(1)
    {
        sleep(1);
    }
    return 0;
}

