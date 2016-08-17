#include "localization.h"

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

#include <QDir>
#include <QFileInfo>
#include <QFile>
#include <QString>
#include <QDirIterator>
#include <QXmlStreamReader>
#include <QDomDocument>



Localization::Localization() : loaded(false),currentLanguage(nullptr)
{

}

Localization::Localization(QString directory)
{
    if(initFromDirectory(directory))
    {
        loaded = true;
        currentLanguage = languages.first();
    }
    else
    {
        loaded = false;
    }
}

bool Localization::initFromDirectory(QString directory)
{
    languages.clear();

    QDir dir(directory);

    if (!dir.exists())
    {
        dir.mkpath(".");
        return false;
    }

    bool atLeastOne = false;

    QDirIterator it(directory, QDirIterator::NoIteratorFlags);
    while (it.hasNext())
    {
        it.next();
        QFileInfo file = it.fileInfo();
        if(file.isFile())
        {
            atLeastOne = (addLanguage(file.absoluteFilePath()) || atLeastOne);
        }
    }

    if(atLeastOne)
    {
        loaded = true;
        currentLanguage = languages.first();
        return true;
    }
    else
    {
        loaded = false;
        currentLanguage = nullptr;
        return false;
    }
}

bool Localization::addLanguage(QString filePath)
{
    QDomDocument doc;
    QFile file(filePath);

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return false;
    }
    if(!doc.setContent(&file))
    {
        return false;
    }
    QDomNodeList list = doc.elementsByTagName(QStringLiteral("FTutor1DLocalization"));
    if(list.size() != 1)
    {
        return false;
    }
    QDomElement root = list.item(0).toElement();

    QString languageName;


    if (root.hasAttribute(QStringLiteral("languageName")))
    {
        languageName = root.attribute("languageName");
    }
    else
    {
        return false;
    }

    Translation* lang = new Translation();

    lang->languageName = languageName;
    lang->countryCode = root.attribute("countryCode");
    lang->data = root;

    languages.insert(languageName,lang);

    return true;
}

QList<QString> Localization::getAvailableLanguages()
{
    return languages.keys();
}


bool Localization::setLanguage(QString language)
{
    if(loaded && languages.contains(language))
    {
        currentLanguage = languages.value(language);
        return true;
    }
    return false;
}

Translation* Localization::getCurrentLanguage() const
{
    return this->currentLanguage;
}

Translation::Translation()
{

}

Translation::Translation(QString name, QString countryCode, QDomElement data)
    :languageName(name),countryCode(countryCode),data(data)
{

}

Translation::Translation(const Translation& other)
{
    this->languageName = other.languageName;
    this->countryCode = other.countryCode;
    this->data = other.data;
}

Translation* Translation::getTranslationForWindow(QString windowName) const
{
    Translation* result = new Translation(*this);

    QDomNodeList children = this->data.elementsByTagName("window");

    for(int i = 0; i < children.count(); i++)
    {
        QDomElement elem = children.item(i).toElement();

        if(elem.hasAttribute("name") && elem.attribute("name") == windowName)
        {
            result->data = elem;
            return result;
        }
    }

    // to do: fix.
    return result;
}


Translation* Translation::getTranslationForElement(QString elementName) const
{
    Translation* result = new Translation(*this);

    QDomNodeList children = this->data.elementsByTagName("UIElement");

    for(int i = 0; i < children.count(); i++)
    {
        QDomElement elem = children.item(i).toElement();

        if(elem.hasAttribute("name") && elem.attribute("name") == elementName)
        {
            result->data = elem;
            return result;
        }
    }

    // to do: fix.
    return result;
}

Translation* Translation::getTranslationForUseCase(QString name) const
{
    Translation* result = new Translation(*this);

    QDomNodeList children = this->data.elementsByTagName("UseCase");

    for(int i = 0; i < children.count(); i++)
    {
        QDomElement elem = children.item(i).toElement();

        if(elem.hasAttribute("name") && elem.attribute("name") == name)
        {
            result->data = elem;
            return result;
        }
    }

    // to do: fix.
    return result;
}


Translation* Translation::getTranslationForElement(int id) const
{
    Translation* result = new Translation(*this);

    QDomNodeList children = this->data.elementsByTagName("UIElement");

    for(int i = 0; i < children.count(); i++)
    {
        QDomElement elem = children.item(i).toElement();

        if(elem.hasAttribute("index") && elem.attribute("index") == QString::number(id))
        {
            result->data = elem;
            return result;
        }
    }

    // to do: fix.
    return result;
}



QString Translation::getTitle() const
{
    QDomNodeList children = this->data.elementsByTagName("title");

    if(!children.isEmpty())
    {
        return children.item(0).toElement().text();
    }
    else
    {
        children = this->data.elementsByTagName("text");
        if(!children.isEmpty())
        {
            return children.item(0).toElement().text();
        }
    }
    return QString("");
}

QString Translation::getText() const
{
    QDomNodeList children = this->data.elementsByTagName("text");

    if(!children.isEmpty())
    {
        return children.item(0).toElement().text();
    }
    else
    {
        children = this->data.elementsByTagName("title");
        if(!children.isEmpty())
        {
            return children.item(0).toElement().text();
        }
    }
    return QString("");
}

QString Translation::getChildElementText(QString elementName) const
{
    QDomNodeList children = this->data.elementsByTagName("UIElement");

    for(int i = 0; i < children.count(); i++)
    {
        QDomElement elem = children.item(i).toElement();

        if(elem.hasAttribute("name") && elem.attribute("name") == elementName)
        {
            QDomNodeList childrenOfElement = elem.elementsByTagName("text");

            if(!childrenOfElement.isEmpty())
            {
                return childrenOfElement.item(0).toElement().text();
            }
            else
            {
               break;
            }
        }
    }
    return QString("");
}

QString Translation::getChildElementText(int elementIndex) const
{
    QDomNodeList children = this->data.elementsByTagName("UIElement");

    for(int i = 0; i < children.count(); i++)
    {
        QDomElement elem = children.item(i).toElement();

        if(elem.hasAttribute("index") && elem.attribute("index") == QString::number(elementIndex))
        {
            QDomNodeList childrenOfElement = elem.elementsByTagName("text");

            if(!childrenOfElement.isEmpty())
            {
                return childrenOfElement.item(0).toElement().text();
            }
            else
            {
               break;
            }
        }
    }
    return QString("");
}


