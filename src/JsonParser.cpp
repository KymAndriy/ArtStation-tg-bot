#include "JsonParser.h"
#include <sstream>
// #include "lib/json.hpp"
#include <iostream>
// #include <boost/json.hpp>
#include "libs/json.hpp"


const std::string JsonParser::_TOKEN_KEY = "BOT_TOKEN";
const std::string JsonParser::_MAP_KEY = "KEYBOARD_MAP";

const std::string JsonParser::_URL_KEY = "FINAL_URL_REGEX";
const std::string JsonParser::_HASH_IMAGE_KEY = "HASH_IMAGE_REGEX";

JsonParser::JsonParser(std::string json_file_name)
{
    _json_file.open(json_file_name, std::ios::in);
    if(!_json_file.is_open())
    {
        throw std::runtime_error("Could not open file");
    }
}


JsonParser::~JsonParser()
{
    if(_json_file.is_open())
    {
        _json_file.close();
    }
}

std::string JsonParser::getToken() const
{
    return _TOKEN;
}

std::map<std::string, std::string> JsonParser::getNameAndUrls() const
{
    return _name_and_urls;
}

int JsonParser::parse()
{

    std::string _json_srt, temp;

    while(!_json_file.eof())
    {
        _json_file >> temp;
        _json_srt.append(temp);
    } 
    try{
        nlohmann::json js = nlohmann::json::parse(_json_srt);

        _TOKEN = js[_TOKEN_KEY];
        _URL_REGEX = js[_URL_KEY];
        _HASH_REGEX = js[_HASH_IMAGE_KEY];
        _name_and_urls = js[_MAP_KEY];
        // boost::json::value v = boost::json::parse(_json_srt);
        // boost::json::object js = v.get_object();
        // // boost::json::array ar = js.at("data").as_array();
        // boost::json::object map = js.at(_MAP_KEY).as_object();
        // for(auto it = map.begin(); it != map.end(); it++)
        // {
        //     _name_and_urls[boost::to_string(it->key())] = boost::to_string(it->value());
        // }
        // // _name_and_urls = ;
        // _TOKEN = boost::to_string(js.at(_TOKEN_KEY).as_string());
        // _URL_REGEX = boost::to_string(js.at(_URL_KEY).as_string());
        // _IMAGE_REGEX = boost::to_string(js.at(_IMAGE_KEY).as_string());
    }catch(std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        throw std::runtime_error("[ERROR] Json parse error\n");
    }
    return 0;
}

std::string JsonParser::getFinalUrlRegex() const
{
    return _URL_REGEX;
}

std::string JsonParser::getHashImageRegex() const
{
    return _HASH_REGEX;
}