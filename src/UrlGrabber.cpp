#include "UrlGrabber.h"
#include <set>
#include <boost/json.hpp>
#include <boost/regex.hpp>

UrlGrabber::UrlGrabber(std::string regex_expression, std::string& json_str)
    :  _regex_str(regex_expression), _json_str(json_str)
{ was_changes = true;}

void UrlGrabber::setRegexExpression(std::string regex_expression)
{
    _regex_str = regex_expression.c_str();
    was_changes = true;
}

void UrlGrabber::setParseStr(std::string to_parse_srt)
{
    _json_str = to_parse_srt;
    was_changes = true;
}

std::vector<std::string> UrlGrabber::getUrls()
{
    if(was_changes)
    {
        grabUrls();
    }
    return _urls;
}

void UrlGrabber::grabUrls()
{
    _urls.clear();
    std::set<std::string> unique;
    boost::regex regex(_regex_str, boost::regex_constants::ECMAScript); 
    boost::sregex_iterator start(_json_str.begin(), _json_str.end(), regex);
    boost::sregex_iterator end;
    for (; start != end; start++)
    {
        boost::smatch match = *start;
        std::string s = match.str();
        unique.insert(s);
    }
    _urls.assign(unique.begin(), unique.end());
}

// void UrlGrabber::grabUrls()
// {
//     _urls.clear();
//     boost::json::value v = boost::json::parse(_json_str);
//     boost::json::object js = v.get_object();
//     boost::json::array ar = js.at("data").as_array();

//     for (auto it = ar.begin(); it != ar.end(); ++it)
//     {
//         _urls.push_back(boost::to_string(it->at("url").as_string()));
//     }
// }
