#include "microtunnelapp.h"
#include "inireader.h"

MicroTunnelApp::MicroTunnelApp()
{

}

bool MicroTunnelApp::start()
{
    LOG(INFO)<<"Starting the Tunnel";
    INIReader reader("/home/gokul/workspace/microtunnel/config.ini"); //TODO : Change it later

    if (reader.ParseError() < 0) {
        std::cout << "Can't load 'config.ini'\n";
        return false;
    }

    std::cout   << reader.GetInteger("protocol", "version", -1) << ", name="
                 << reader.Get("user", "name", "UNKNOWN") << ", email="
                 << reader.Get("user", "email", "UNKNOWN") << ", multi="
                 << reader.Get("user", "multi", "UNKNOWN") << ", pi="
                 << reader.GetReal("user", "pi", -1) << ", active="
                 << reader.GetBoolean("user", "active", true) << "\n";

    return true;
}
bool MicroTunnelApp::stop()
{
    LOG(INFO)<<"Stopping the Tunnel";
    return true;
}
