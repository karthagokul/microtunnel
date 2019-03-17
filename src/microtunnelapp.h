#ifndef MICROTUNNELAPP_H
#define MICROTUNNELAPP_H

#include "logger.h"
#include "configurator.h"
#include "tunnelsession.h"
#include <vector>
#include <map>

class MicroTunnelApp
{
public:
    MicroTunnelApp();
    bool start();
    bool stop();
private:
    bool readGeneralConfiguration();
private:
    std::vector<TunnelSession *> mSessions;
    Configurator mConfig;
};

#endif // MICROTUNNELAPP_H
