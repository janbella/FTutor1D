#ifndef LOCALIZATION_H
#define LOCALIZATION_H

#include <vector>
#include <map>
#include <QString>


class Localization
{
private:
    std::vector<QString> languages;
    std::map<QString,QString> entries;

    QString currentLanguage;
    bool loaded;
public:
    Localization();
    Localization(const char* directory);

    const QString& operator()(const QString& guiElement, const QString& sentence) const;

    bool loadLanguages(QString directory);

    void loadFile(QString directory, QString filename);

    inline const std::vector<QString>& getLanguages() const
    {
        return languages;
    }


};

#endif // LOCALIZATION_H
