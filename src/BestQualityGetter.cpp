#include "BestQualityGetter.h"
#include <regex>


BestQualityGetter::BestQualityGetter(std::vector<std::string> images)
{
    _urls = images;
}

std::vector<std::string> BestQualityGetter::extract()
{
    std::string a = "(\\w\\d+\\/)+((4k)|(large)|(small)|(medium)|)\\/";
    std::string b = "(\\w\\d+\\/)";
    std::regex base_regex(b);

    std::string previous;
    std::smatch base_match_1;
    std::smatch base_match_2;

    size_t size = _urls.size()-2;

    for(size_t i = 0; i < size; i++)
    {
        std::regex_match(_urls[i], base_match_1, base_regex);
        std::regex_match(_urls[i+1], base_match_2, base_regex);
        if(base_match_1[0] == base_match_2[0])
        {
            if(_urls[i] > _urls[i+1])
        }
        // previous = base_match[0];
    }
    // a.find()
}
// Forbiden url, Needed to check in telegram bot
//https://cdna.artstation.com/p/assets/images/images/001/578/772/20151130150605/4k/chris-hodgson-church.jpg