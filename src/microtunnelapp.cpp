#include "microtunnelapp.h"

structlog LOGCFG = {};

MicroTunnelApp::MicroTunnelApp()
{
    //Basic Logging
    LOGCFG.headers = true;
    LOGCFG.level = DEBUG;
}

bool MicroTunnelApp::readGeneralConfiguration()
{

    if(mConfig.systemconfig().debuglog==true)
    {
        LOGCFG.level = DEBUG;
        LOG(INFO)<<"Debug Logs : True";
    }
    else
    {
        LOG(INFO)<<"Debug Logs : False";
    }

    if(mConfig.systemconfig().foreground==true)
    {
        LOG(INFO)<<"Run as service : True";
    }
    else
    {
        LOG(INFO)<<"Run as service : False";
    }
    return true;
}

bool MicroTunnelApp::start()
{
    readGeneralConfiguration();

    std::map<std::string,TunnelConfig *> tconfs=mConfig.tunnelconfig();
    for (auto const& x : tconfs)
    {
        LOG(INFO)<<x.first;
        TunnelConfig *tIt=x.second;
        LOG(INFO)<<"Cipher : "<<tIt->cipher;
        LOG(INFO)<<"Source Host : "<<tIt->sourcehost;
        LOG(INFO)<<"Source Port : "<<tIt->sourceport;
        LOG(INFO)<<"Target Host : "<<tIt->targethost;
        LOG(INFO)<<"Target Port : "<<tIt->targetport;
    }
    return true;
}

bool MicroTunnelApp::stop()
{
    LOG(INFO)<<"Stopping the Tunnel";
    return true;
}
