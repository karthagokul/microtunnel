#include <iostream>
#include <signal.h>
#include "tcpclientsession.h"
#include "tcpserversession.h"
#include "logger.h"

#include "microtunnelapp.h"

using namespace std;
MicroTunnelApp app;

void sig_handler(int signo)
{
    if (signo == SIGUSR1)
        printf("received SIGUSR1\n");
    else if (signo == SIGKILL)
        printf("received SIGKILL\n");
    else if (signo == SIGSTOP)
        printf("received SIGSTOP\n");
    app.stop();
    exit(0);
}

int main()
{
    //TODO : Better way
    if (signal(SIGINT, sig_handler) == SIG_ERR)
        LOG(WARN)<<"Failed:SIGINT Subscription";
    if (signal(SIGTERM, sig_handler) == SIG_ERR)
        LOG(WARN)<<"Failed:SIGTERM Subscription";
    if (signal(SIGABRT, sig_handler) == SIG_ERR)
        LOG(WARN)<<"Failed:SIGABRT Subscription";


    app.start();

    //Main event loop
    while(1)
    {
        sleep(1);
    }
    return 0;
}

