#include "JsonParser.h"
#include <iostream>
#include <string>

#include <fstream>
#include <vector>
#include "UrlGrabber.h"

int main()
{
    JsonParser p("/home/andrii/temp_project/ArtStation-tg-bot/bot_configs.json");
    p.parse();
    
    std::string str;
    std::fstream fst("/home/andrii/temp_project/ArtStation-tg-bot/artwork.json", std::ios::in);
    std::string temp;
    while(!fst.eof())
    {
        fst >> temp;
        str.append(temp);
    } 
    UrlGrabber grab;//(p.getImageUrlRegex(), str);
    grab.setParseStr(str);
    grab.setRegexExpression(p.getImageRegex());
    std::vector<std::string> _urls = grab.getUrls();


    for(size_t i = 0; i < _urls.size(); i++)
    {
        std::cout << _urls[i] << std::endl;
    }
    
    fst.close();
    return 0;
}