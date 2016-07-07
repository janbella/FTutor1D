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

Localization::Localization()
{

}

Localization::Localization(const char *directory)
{
    loadLanguages(directory);

}

const QString& Localization::operator()(const QString& guiElement, const QString& sentence) const
{
    if(loaded)
    {
        QString key = guiElement + "@" + sentence;
        if(entries.find(key) != entries.end())
        {
            return entries.at(key);
        }
    }
    return sentence;
}

bool Localization::loadLanguages(QString directory)
{
    QDir dir(directory);

    if (!dir.exists())
    {
        dir.mkpath(".");
        return false;
    }

    QDirIterator it(directory, QDirIterator::NoIteratorFlags);
    while (it.hasNext())
    {
        it.next();
        QFileInfo file = it.fileInfo();
        if(file.isFile())
        {
            languages.push_back(file.fileName());
        }
    }
    return true;
}

void Localization::loadFile(QString directory, QString filename)
{
    QFile file(directory + QString("/") + filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
       // error message here
       return;
    }
    entries.clear();
    while (!file.atEnd())
    {
        QString line = file.readLine();
        if( line.endsWith("\n") )
        {
            line.truncate( line.length() - 1 );
        }
        QStringList list = line.split("=");
        entries.insert(std::pair<QString, QString>(list[0],list[1]));
    }

//    std::ifstream input(directory + QString("/") + filename);
//    while(input)
//    {
//        QString line = in.readLine();
//            // now, line will be a string of the whole line, if you're trying to read a CSV or something, you can split the string
//            QStringList list = line.split(",");

//        QString key;
//        std::getline(input,key,'=');
//        QString value;
//        std::getline(input,value);
//        //entries.insert(key,value);
//        entries.insert(std::pair<QString, QString>(key,value));
//    }
    loaded = true;
    currentLanguage = filename;
}



