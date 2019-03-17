#include "socket.h"
#include <iostream>

SessionError Session::getError()
{
    LOG_FUNCTION_NAME;
    SessionError errorCode=Unknown;
    if(mSockFd!=-1)
    {
        int error = 0;
        socklen_t len = sizeof (error);
        int retval = getsockopt (mSockFd, SOL_SOCKET, SO_ERROR, &error, &len);
        if (retval != 0)
        {
            /* there was a problem getting the error code */
            LOG(ERROR)<< "Error getting socket error code:"<<strerror(retval);
            errorCode=Unknown;
        }
        else
        {
            if(error!=0)
            {
                //Check error value https://www.xinotes.net/notes/note/1793/
                LOG(ERROR)<<"Socket Error Code "<<error<<": "<<strerror(error);
                errorCode=ServerNotReachable; //Temporary , This need to be fine tuned
            }
            else
            {
                errorCode=NoError;
            }
        }

    }
    return errorCode;
}

bool Session::writeBuffer(const char *aBuffer)
{
    LOG_FUNCTION_NAME;
    if(mSockFd<=0)
    {
        return false;
    }
    if(write(mSockFd, aBuffer, sizeof(aBuffer))<=0)
    {
        LOG(ERROR)<<"Sending Buffer failed";
        return false;
    }
    return true;
}


bool  Session::cleanup()
{
    if(status()==Disconnected)
    {
        LOG(WARN)<<"Socket is already disconnected";
        return false;
    }
    LOG(DEBUG)<<"Socket ID : "<<mSockFd;
    if (mSockFd!=-1)
    {
        LOG(DEBUG)<<"Socket ID : "<<mSockFd;
        getError(); // first clear any errors, which can cause close to fail
        if (shutdown(mSockFd, SHUT_RDWR) < 0) // secondly, terminate the 'reliable' delivery
            if (errno != ENOTCONN && errno != EINVAL) // SGI causes EINVAL
                LOG(ERROR)<<"shutdown";
        if (close(mSockFd) < 0) // finally call close()
            LOG(ERROR)<<"close";
    }
    setStatus(Disconnected);
    return true;
}
