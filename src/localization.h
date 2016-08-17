#ifndef LOCALIZATION_H
#define LOCALIZATION_H

#include <QMap>
#include <QVector>
#include <QString>
#include <QDomDocument>
#include <QDir>
#include <QDirIterator>

struct Translation
{
    QString languageName;
    QString countryCode;
    QDomElement data;

    Translation();

    Translation(QString languageName, QString countryCode, QDomElement data);

    Translation(const Translation& other);

    Translation* getTranslationForWindow(QString windowName) const;

    Translation* getTranslationForElement(QString elementName) const;

    Translation* getTranslationForUseCase(QString name) const;

    Translation* getTranslationForElement(int id) const;

    QString getTitle() const;

    QString getText() const;

    QString getChildElementText(QString elementName) const;

    QString getChildElementText(int elementIndex) const;

};

class Localization
{
private:
    QMap<QString,Translation*> languages;
    bool loaded;
    Translation* currentLanguage;

public:
    Localization();

    Localization(QString directory);

    QList<QString> getAvailableLanguages();

    bool setLanguage(QString language);

    bool initFromDirectory(QString directory);

    Translation* getCurrentLanguage() const;

private:

    bool addLanguage(QString filePath);
};


#endif // LOCALIZATION_H
