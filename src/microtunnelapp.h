#ifndef MICROTUNNELAPP_H
#define MICROTUNNELAPP_H

#include "tcpclientsession.h"
#include "tcpserversession.h"
#include "logger.h"

#include <map>

class TunnelSession
{
    //a network instance
};

class MicroTunnelApp
{
public:
    MicroTunnelApp();
    bool start();
    bool stop();

private:
    std::map<std::string,TunnelSession *> mSessions;
};

#endif // MICROTUNNELAPP_H
