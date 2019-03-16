#ifndef CONFIGURATOR_H
#define CONFIGURATOR_H

#include <iostream>
#include <map>
#include "logger.h"
#include "inireader.h"

#define GENERAL_HEADER "general"
#define GENERAL_ENABLE_LOG "enable-debug-log"
#define GENERAL_FOREGROUND "foreground"

#define TUNNEL_CIPHER "cipher"
#define TUNNEL_ISSERVER "server"
#define TUNNEL_SOURCEPORT "source-port"
#define TUNNEL_TARGETPORT "target-port"

//default configs
struct GeneralConfig
{
   bool debuglog=false;
   bool foreground=false;
};

struct TunnelConfig
{
    std::string cipher="";
    bool server=true;
    uint sourceport=8080;
    uint targetport=9090  ;
};

class Configurator
{
public:
    Configurator();
    bool init();
    GeneralConfig systemconfig() const
    {
        return mGenconfig;
    }

private:
    INIReader mIniReader;
    GeneralConfig mGenconfig;
    std::map<std::string,TunnelConfig> mTunnelConfigs;
};

#endif // CONFIGURATOR_H
