#include "JsonReceiver.h"

int JsonReceiver::operator()(std::string url, std::string &to_return)
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
