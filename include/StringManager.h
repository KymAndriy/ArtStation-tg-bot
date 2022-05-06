#ifndef IMG_BOT_STRING_MANAGER_H
#define IMG_BOT_STRING_MANAGER_H

#include <string>
#include <map>
#include <vector>

namespace StringManager
{   
    inline static void extractHash(std::vector<std::string>& urls)
    {
        for(auto &iter: urls)
        {
            iter = "https://www.artstation.com/projects" +
                        iter.substr(iter.find_last_of('/'), iter.size()) +".json";
        }
    }

    inline static void replaceCharacters(std::string& source,
            std::vector<char> old_val, char new_val = '_')
    {
        for(auto i: old_val){
            std::replace(source.begin(), source.end(), i, new_val);
        }
    }
};

#endif