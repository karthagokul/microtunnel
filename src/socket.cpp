#include "socket.h"
#include <iostream>

SessionError Session::getError()
{
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
    if(write(mSockFd, aBuffer, sizeof(aBuffer))<=0)
    {
        LOG(ERROR)<<"Sending Buffer failed";
        return false;
    }
    return true;
}
