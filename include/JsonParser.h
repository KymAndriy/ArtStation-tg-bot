#ifndef IMG_BOT_JSON_PARSER_H
#define IMG_BOT_JSON_PARSER_H

#include <string>
#include <map>
#include <fstream>

class JsonParser 
{
public:
    JsonParser(std::string json_file_name);

    ~JsonParser();

    std::string getToken() const;

    std::string getFinalUrlRegex() const;
    
    std::string getHashImageRegex() const;

    std::map<std::string, std::string> getNameAndUrls() const;

    int parse();

private:
    std::fstream _json_file;
    std::string _TOKEN;
    std::string _HASH_REGEX;
    std::string _URL_REGEX;
    std::map<std::string, std::string> _name_and_urls;    

    static const std::string _TOKEN_KEY; 
    static const std::string _MAP_KEY;
    static const std::string _URL_KEY;
    static const std::string _HASH_IMAGE_KEY;
};

#endif