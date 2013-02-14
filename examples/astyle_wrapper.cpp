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
    // allocate internal buffer
    size_t len = content.size();
    char *textIn = new(std::nothrow) char[len + 1];
    if (!textIn)
        return false;
    // copy to internal buffer
#ifdef WIN32
#pragma warning(push)  // FIX
#pragma warning(disable: 4996)
    size_t copied = content.copy(textIn, len);
#pragma warning(pop)
#else
    size_t copied = content.copy(textIn, len);
#endif
	if (copied != len)
    {
        delete[] textIn;
        return false;
    }
    textIn[len] = '\0';
    // call astyle
    error = false;
    char *textOut = AStyleMain(textIn, options, handleError, allocMemory);
    delete[] textIn;
    // check error
    if (error)
    {
        if (textOut)
            delete[] textOut;
        return false;
    }
    if (!textOut)
        return false;
    // return result
    std::string result(textOut, strlen(textOut));
	delete[] textOut;
    content.swap(result);
	return true;
}