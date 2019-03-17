/*
 * File:   Log.h
 * Author: Alberto Lepe <dev@alepe.com>
 *
 * Created on December 1, 2015, 6:00 PM
 */

#ifndef LOG_H
#define LOG_H

#include <iostream>
#include <ostream>

using namespace std;

namespace Color {
enum Code {
    FG_RED      = 31,
    FG_GREEN    = 32,
    FG_BLUE     = 34,
    FG_DEFAULT  = 39,
    BG_RED      = 41,
    BG_GREEN    = 42,
    BG_BLUE     = 44,
    BG_DEFAULT  = 49
};
class Modifier {
    Code code;
public:
    Modifier(Code pCode) : code(pCode) {}
    friend std::ostream&
    operator<<(std::ostream& os, const Modifier& mod) {
        return os << "\033[" << mod.code << "m";
    }
};
}

enum typelog {
    INFO = 0x1,
    ERROR = 0x2,
    WARN = 0x4,
    DEBUG = 0x8,
    DEFAULT_LOG_LEVEL =  INFO | ERROR | WARN,
    ALL_LOG_LEVEL = INFO | ERROR | WARN | DEBUG

};

struct structlog {
    bool headers = false;
    typelog level = WARN;
    int debug_depth=1;
};

extern structlog LOGCFG;
static Color::Modifier red(Color::FG_RED);
static Color::Modifier def(Color::FG_DEFAULT);
static Color::Modifier blue(Color::FG_BLUE);
static Color::Modifier green(Color::FG_GREEN);

class LOG {
public:
    LOG() {}
    LOG(typelog type,int debug_depth=0) {
        msglevel = type;
        depth=debug_depth;
        if(LOGCFG.headers) {

            switch(type)
            {
            case DEBUG:
                operator << (def);
                break;
            case INFO:
                operator << (green);
                break;
            case WARN:
                operator << (blue);
                break;
            case ERROR:
                operator << (red);
                break;
            default:
                operator << (def);
                break;
            }

            operator << ("["+getLabel(type)+"]");
            operator << (def);
        }
    }
    ~LOG() {
        if(opened) {
            cout << endl;
        }
        opened = false;
    }
    template<class T>
    LOG &operator<<(const T &msg) {
        if(msglevel & LOGCFG.level) {
            if(LOGCFG.debug_depth>=depth)
            {
                cout << msg;
                opened = true;
            }

        }
        return *this;
    }
private:
    bool opened = false;
    int depth=0;
    typelog msglevel = DEBUG;
    inline string getLabel(typelog type) {
        string label;
        switch(type) {
        case DEBUG: label = "DEBUG"; break;
        case INFO:  label = "INFO "; break;
        case WARN:  label = "WARN "; break;
        case ERROR: label = "ERROR"; break;
        }
        return label;
    }
};

#define DEBUG_LEVEL_1 1
#define DEBUG_LEVEL_2 2
#define DEBUG_LEVEL_3 3
#define LOG_FUNCTION_NAME { LOG(DEBUG,DEBUG_LEVEL_3) << __PRETTY_FUNCTION__;}

#endif  /* LOG_H */
