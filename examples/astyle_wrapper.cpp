/*
 *  Version: 1.0
 *  Date:    2012/12/12
 *  Author:  Ruzzz ruzzzua[]gmail.com
 */

#include <string>

#include "astyle_wrapper.h"

extern "C"
char *AStyleMain(const char *pSourceIn,
                 const char *pOptions,
                 void (*fpError)(int, char *),
                 char * (*fpAlloc)(unsigned long));

bool error;

void handleError(int errorNumber, char *errorMessage)
{
    error = true;
}

char *allocMemory(unsigned long bytesNeeded)
{
    return new(std::nothrow) char[bytesNeeded];
}

bool astyle(std::string &content, const char *options)
{
    size_t len = content.size();
    char *textIn = new(std::nothrow) char[len + 1];
    if (!textIn)
        return false;
#ifdef WIN32
#pragma warning(push)  // FIX
#pragma warning(disable: 4996)
    size_t copied = content.copy(textIn, len);
#pragma warning(pop)
#else
    size_t copied = content.copy(textIn, len);
#endif

    if (copied != len)
        goto exit;
    textIn[len] = '\0';
    error = false;
    char *textOut = AStyleMain(textIn, options, handleError, allocMemory);
    if (error)
    {
        if (textOut)
            delete [] textOut;
        goto exit;
    }
    if (textOut) {
        content.assign(textOut, strlen(textOut));
        delete [] textOut;
        delete [] textIn;
        return true;
    }
exit:
    delete [] textIn;
    return false;
}