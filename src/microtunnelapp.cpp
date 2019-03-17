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
        LOGCFG.level = ALL_LOG_LEVEL;
        LOG(INFO)<<"Debug Logs : True";
    }
    else
    {
        LOG(INFO)<<"Debug Logs : False";
        LOGCFG.level = DEFAULT_LOG_LEVEL;
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
        TunnelSession *session=new TunnelSession(tIt);
        mSessions.push_back(session);
    }

    return true;
}

bool MicroTunnelApp::stop()
{
    //LOG(DEBUG)<<"Stopping the Tunnel";

    std::vector<TunnelSession*>::iterator it = mSessions.begin();
    for (; it != mSessions.end(); ++it) {
        delete *it;
    }
    mSessions.clear();
   // LOG(DEBUG)<<"Tunnel Session Count"<<mSessions.size();
    return true;
}
