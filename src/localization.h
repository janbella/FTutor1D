#ifndef LOCALIZATION_H
#define LOCALIZATION_H

/**
 * @file localization.h
 * @author Ján Bella <xbella1@fi.muni.cz>
 *
 * The file provides definitions of classes Localization and Translation.
 */

#include <QMap>
#include <QVector>
#include <QString>
#include <QDomDocument>
#include <QDir>
#include <QDirIterator>

namespace FT1D
{
/**
 * @brief The Translation struct is a language version of all texts in the application.
 */
struct Translation
{
    // attributs

    // language name as it should be displayed in the menu, e.g. English
    QString languageName;

    // country code, e.g. EN-US. This is currently not used.
    QString countryCode;

    // contains all the data read from the XML file, which are translation of a specific element
    QDomElement data;

    /**
     * @brief Translation constructs the empty object
     */
    Translation();

    /**
     * @brief Translation constructs the object given all important properties
     * @param languageName name of the language version
     * @param countryCode country code corresponding to this language version
     * @param data translation data in form of a DOM tree
     */
    Translation(const QString& languageName, const QString& countryCode, const QDomElement& data);

    /**
     * @brief Translation copy constructor
     * @param other an object to copy
     */
    Translation(const Translation& other);

    /**
     * @brief getTranslationForWindow given @a windowName, obtains the subtree of the data starting with window element having the name equal to the @a windowName
     * @param windowName name of the window for which to acquire the translation
     * @return a pointer to newly allocated instance of translation, or nullptr if such translation was not found
     */
    Translation* getTranslationForWindow(const QString&  windowName) const;

    /**
     * @brief getTranslationForElement obtains the subtree of the data starting with UIElement  having the name equal to the @a elementName
     * @param elementName name of the element for which to acquire the translation
     * @return a pointer to newly allocated instance of translation, or nullptr if such translation was not found
     */
    Translation* getTranslationForElement(const QString&  elementName) const;

    /**
     * @brief getTranslationForUseCase obtains the subtree of the data starting with UseCase element having the name equal to the @a name
     * @param name name of the use case for which to acquire the translation
     * @return a pointer to newly allocated instance of translation, or nullptr if such translation was not found
     */
    Translation* getTranslationForUseCase(const QString&  name) const;

    /**
     * @brief getTranslationForElement obtains the subtree of the data starting with UIElement having the index equal to the @a id
     * @param id id of the element for which to acquire the translation
     * @return a pointer to newly allocated instance of translation, or nullptr if such translation was not found
     */
    Translation* getTranslationForElement(const int id) const;

    /**
     * @brief getTitle obtains the text contained between child <title> and <title/> tag
     *  if it has no title tag, returns the content of <text> and </text> tag, or an empty string
     * @return
     */
    QString getTitle() const;

    /**
     * @brief getText obtains the text contained between child <text> and <text/> tag
     * @return demanded string, or empty string in case of failure
     */
    QString getText() const;

    /**
     * @brief getChildElementText obtains text of the child element
     * @param elementName element name, for which to get the text
     * @return text of the child element, of an empty string in case of failure
     */
    QString getChildElementText(const QString& elementName) const;

    /**
     * @brief getChildElementText obtains text of the child element
     * @param elementIndex element index, for which to get the text
     * @return text of the child element, of an empty string in case of failure
     */
    QString getChildElementText(const int elementIndex) const;

};

/**
 * @brief The Localizations class is responsible for managing the language versions (Translations)
 */
class Localizations
{
private:
    // the map, containing names and translation instances of all loaded languages.
    QMap<QString,Translation*> languages;
    // true signifies yes
    bool loaded;
    // pointer to the currently set language
    Translation* currentLanguage;

public:
    /**
     * @brief Localizations constructs an empty instance
     */
    Localizations();

    /**
     * @brief Localizations reads the @a directory and fills the attributes based on files found in that directory
     * @param directory a path to the directory containing the translations
     */
    Localizations(const QString& directory);

    /**
     * @brief getAvailableLanguages returns list of all available language versions
     * @return list of all available language versions
     */
    QList<QString> getAvailableLanguages() const;

    /**
     * @brief setLanguage sets the language @a language as the current
     * @param language name of the language to set
     * @return true in case of success, false if no language with name @a language exists
     */
    bool setLanguage(const QString& language);

    /**
     * @brief initFromDirectory implements reading the directory and setting up the attributes based on what's read
     * @param directory a path to the directory to process
     * @return bool in case of success, false in case of failure
     */
    bool initFromDirectory(const QString& directory);

    /**
     * @brief getCurrentLanguage returns poitner to the  selected language
     * @return pointer to the selected language
     */
    Translation* getCurrentLanguage() const;

private:
    /**
     * @brief addLanguage checks and reads the file at @a filePath. If it's a valid Ftutor1d localization file, it's added to the languages map in form of Translation instance
     * @param filePath path of file to read
     * @return true if the file was valid and added, false if not
     */
    bool addLanguage(const QString& filePath);
};
}

#endif // LOCALIZATION_H
