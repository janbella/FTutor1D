#ifndef LOCALIZATION_H
#define LOCALIZATION_H

#include <vector>
#include <map>
#include <string>


class Localization
{
private:
    std::vector<std::string> languages;
    std::map<std::string,std::string> entries;

public:
    Localization();
    Localization(const char* directory);

    std::string operator()(std::string& key);


};

#endif // LOCALIZATION_H
