#ifndef MICROTUNNELAPP_H
#define MICROTUNNELAPP_H

#include "tcpclientsession.h"
#include "tcpserversession.h"
#include "logger.h"
#include "configurator.h"
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
    bool readGeneralConfiguration();
private:
    std::map<std::string,TunnelSession *> mSessions;
    Configurator mConfig;
};

#endif // MICROTUNNELAPP_H
