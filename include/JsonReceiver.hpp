#ifndef IMG_BOT_JSON_RECEIVER_H
#define IMG_BOT_JSON_RECEIVER_H

#include <curl/curl.h>
#include <string>

class JsonReceiver
{
public:
    inline static int get(std::string url, std::string &to_return)
    {
        CURL *curl;
        curl = curl_easy_init();
        curl_easy_setopt(curl, CURLOPT_URL, &url[0]);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, JsonReceiver::write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &to_return);
        CURLcode code = curl_easy_perform(curl);
        if (code != 0)
        {
            return code;
        }
        curl_easy_cleanup(curl);
        return 0;
    }

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