#include "configurator.h"


Configurator::Configurator()
{
    if(!init())
    {
        LOG(ERROR)<<"Unable to read configuration, Using defaults";
    }
}


bool Configurator::init()
{
    mIniReader.setFileName("/home/gokul/workspace/microtunnel/config.ini"); //TODO : Change it later

    if (mIniReader.ParseError() < 0) {
        std::cout << "Can't load 'config.ini'\n";
        return false;
    }
    LOG(DEBUG)<<" ";
    std::set<std::string> sections = mIniReader.Sections();
    for (std::set<std::string>::iterator it = sections.begin(); it != sections.end(); ++it)
    {
        std::string sectionheader=*it;
        if(sectionheader== GENERAL_HEADER) // General Config
        {
            LOG(INFO)<<"General Section";
            if(mIniReader.GetInteger(GENERAL_HEADER,GENERAL_ENABLE_LOG,0)==1)
            {
                //Enable Debug Logs case
                mGenconfig.debuglog=true;
            }
            if(mIniReader.GetInteger(GENERAL_HEADER,GENERAL_FOREGROUND,0)==1)
            {
                //Enable Debug Logs case
                mGenconfig.foreground=true;
            }
        }
        else // Tunnel Config
        {
            LOG(INFO)<<"Tunnel Configuration : " <<sectionheader;
        }
    }
    LOG(DEBUG)<<"";

    /*std::cout   << reader.GetInteger("protocol", "version", -1) << ", name="
                 << reader.Get("user", "name", "UNKNOWN") << ", email="
                 << reader.Get("user", "email", "UNKNOWN") << ", multi="
                 << reader.Get("user", "multi", "UNKNOWN") << ", pi="
                 << reader.GetReal("user", "pi", -1) << ", active="
                 << reader.GetBoolean("user", "active", true) << "\n";*/

    return true;
}

