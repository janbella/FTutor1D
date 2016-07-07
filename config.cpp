#include "config.h"

#include <fstream>
#include <algorithm>

#include <cctype>
#include <QFileInfo>
#include <QFile>


Config::Config()
{
    currentLanguage.clear();
    dataFolder.clear();
    localizationFolder.clear();

    QFileInfo check_file(Config::config_file_path);
    if(!check_file.exists())
    {
        createConfigurationFile();
    }
    else
    {
        loadConfigurationFile();
    }
}


bool Config::createConfigurationFile()
{
    std::ofstream configFile(Config::config_file_path);

    if(configFile)
    {
        configFile << "data folder = ./data/" << std::endl;
        configFile << "localization folder = ./translation/" << std::endl;
        configFile.close();
        return true;
    }
    else
    {
        return false;
    }
}

void Config::clearToken(std::string& token)
{
    while(isspace(token.back()))
    {
        token.pop_back();
    }
    int i = 0;
    while(isspace(token[i]))
    {
        i++;
    }
    token.erase(0,i);
    std::transform(token.begin(), token.end(), token.begin(), tolower);

}

bool Config::loadConfigurationFile()
{
    std::ifstream configFile(Config::config_file_path);

    while(configFile)
    {
        std::string input;
        std::getline(configFile,input,'=');
        clearToken(input);
        if(input =="data folder")
        {
            std::getline(configFile,input);
            this->dataFolder = QString(input.c_str());
        }
        else if (input == "localization folder")
        {
            std::getline(configFile,input);
            this->localizationFolder = QString(input.c_str());
        }
        else if(input == "chosen language")
        {
            std::getline(configFile,input);
            this->currentLanguage = QString(input.c_str());
        }
    }
    configFile.close();
    if(dataFolder.isEmpty() || localizationFolder.isEmpty())
    {
        return false;
    }
    else
    {
        return true;
    }
}

void Config::saveConfig()
{
    std::ofstream configFile(Config::config_file_path);

    if(configFile)
    {
        configFile << "data folder=" << dataFolder.toStdString() << std::endl;
        configFile << "localization folder=" << localizationFolder.toStdString() << std::endl;
        configFile << "chosen language=" << currentLanguage.toStdString() << std::endl;
        configFile.close();
    }
}

Config::~Config()
{
    saveConfig();
}
