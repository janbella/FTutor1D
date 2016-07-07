#ifndef CONFIG_H
#define CONFIG_H

#include <QString>

class Config
{
private:
    static constexpr const char* config_file_path = "config.txt";

    QString localizationFolder;

    QString dataFolder;

    bool createConfigurationFile();

    bool loadConfigurationFile();

    void clearToken(std::string& token);

    void saveConfig();


public:
    Config();

    QString currentLanguage;

    inline const QString& getLocalizationFolder() const
    {
        return this->localizationFolder;
    }

    inline const QString& getDataFolder() const
    {
        return this->dataFolder;
    }

    ~Config();

};

#endif // CONFIG_H
