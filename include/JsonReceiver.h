#ifndef IMG_BOT_JSON_RECEIVER_H
#define IMG_BOT_JSON_RECEIVER_H

#include <curl/curl.h>
#include <string>

class JsonReceiver
{
public:
    int operator()(std::string url, std::string &to_return);

private:

inline static size_t write_callback(char *ptr, size_t size, size_t nmemb, void *stream)
{
    if (stream)
    {
        ((std::string *)stream)->append(ptr, size * nmemb);
    }
    return size * nmemb;
}

};

#endif