#include "microtunnelapp.h"
#include "configurator.h"

structlog LOGCFG = {};


MicroTunnelApp::MicroTunnelApp()
{
    //Basic Logging
    LOGCFG.headers = true;
    LOGCFG.level = ERROR;

}

bool MicroTunnelApp::start()
{
    LOG(INFO)<<"Configuration Summary";
    Configurator config;
    if(config.systemconfig().debuglog==true)
    {
        LOGCFG.level = DEBUG;
        LOG(INFO)<<"Debug Logs : True";
    }
    else
    {
        LOG(INFO)<<"Debug Logs : False";
    }

    if(config.systemconfig().foreground==true)
    {
        LOG(INFO)<<"Run as service : True";
    }
    else
    {
        LOG(INFO)<<"Run as service : False";
    }


    return true;
}
bool MicroTunnelApp::stop()
{
    LOG(INFO)<<"Stopping the Tunnel";
    return true;
}
