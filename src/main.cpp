#include <iostream>
#include "socket.h"
#include "logger.h"

using namespace std;

class TestProxy:public TcpServerSessionListener
{
public:
    void onError(const SessionError &aErrorCode)
    {
    }
    void newConnection(const TcpClientSession *a)
    {

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

    LOGCFG.headers = false;
    LOGCFG.level = DEBUG;

    TestProxy t;
    t.start();

    while(1)
    {
        sleep(1);
    }
    return 0;
}

