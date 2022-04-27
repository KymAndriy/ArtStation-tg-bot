#ifndef IMG_BOT_BEST_QUALITY_GETTER_H
#define IMG_BOT_BEST_QUALITY_GETTER_H

#include <string>
#include <vector>
// #include <string>

class BestQualityGetter
{

    BestQualityGetter() = default;
    BestQualityGetter(std::vector<std::string> images);

    std::vector<std::string> extract();
private:
    std::vector<std::string> _urls;

};


#endif