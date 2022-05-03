#ifndef IMG_URL_GRABBER_H
#define IMG_URL_GRABBER_H

#include <regex>
#include <vector>
#include <string>


class UrlGrabber
{
public:
    UrlGrabber() = default;
    UrlGrabber(const UrlGrabber& ) = delete;
    UrlGrabber(const UrlGrabber&& ) = delete;

    UrlGrabber(std::string regex_expression, std::string& json_str);

    void setRegexExpression(std::string expression);

    void setParseStr(std::string& json_str);

    std::vector<std::string> getUrls();


private:
    void grabUrls();


private:
    std::string _regex_str;
    std::string _json_str;
    std::vector<std::string> _urls;
    bool was_changes;
};

#endif
