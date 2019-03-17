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
#define TUNNEL_SOURCEHOSTNAME "source-host-name"
#define TUNNEL_TARGETHOSTNAME "target-host-name"

//default configs
struct GeneralConfig
{
    bool debuglog=false;
    bool foreground=false;
};

struct TunnelConfig
{
    std::string name;
    std::string cipher="";
    bool server=true;
    uint sourceport=8080;
    uint targetport=9090  ;
    std::string sourcehost="127.0.0.1";
    std::string targethost="127.0.0.1";
};

/*!
 * \brief The Configurator class : The class is responsible for the system wide configuration fetched from an external ini file.
 */

class Configurator
{
public:
    Configurator();
    bool init();
    GeneralConfig systemconfig() const
    {
        return mGenconfig;
    }

    std::map<std::string,TunnelConfig *> tunnelconfig() const
    {
        return mTunnelConfigs;
    }


private:
    INIReader mIniReader;
    GeneralConfig mGenconfig;
    std::map<std::string,TunnelConfig *> mTunnelConfigs;
};

#endif // CONFIGURATOR_H
