#include "dictionary.h"
#include <QVariantMap>

bool operator == ( const DictionaryElement &element1, const DictionaryElement &element2 )
{
    if ( (element1.word == element2.word) && (element1.translate == element2.translate) )
        return true;
    else
        return false;
}

DictionaryElement::DictionaryElement (const QString &r_word, const QString &r_translate) :
    word(r_word),
    translate(r_translate)
{

}

Dictionary::Dictionary()
{

}

////
void Dictionary::setDict(const QMap<QString, QString> &dict)
{
    // ??? вставить проверку валидности словаря
    m_dict = dict;
}

QMap<QString, QString> Dictionary::getDict() const
{
    return m_dict;
}

bool Dictionary::setWord(const QString &word, const QString &translate)
{
    if ((word != "") && (translate != "")) {
       m_dict[word] = translate;
       return true;
    }
    return false;
}

QString Dictionary::getWord(const QString &translate) const
{
    return m_dict.key(translate, QString());
}

QString Dictionary::getTranslate(const QString &word) const
{
    return (m_dict.count(word) < 1) ? QString() : m_dict[word];
}

void Dictionary::removeWord(const QString &word)
{
    m_dict.remove(word);
}

////
void Dictionary::setGroups(const QMap<QString, QMap<QString, QString>> &groups)
{
    // ??? вставить проверку валидности групп
    m_groups = groups;
}

QMap<QString, QMap<QString, QString>> Dictionary::getGroups() const
{
    return m_groups;
}

void Dictionary::setGroup(const QString &nameGroup, const QMap<QString, QString> &groupDict)
{
    m_groups[nameGroup] = groupDict;
}

QMap<QString, QString> Dictionary::getGroup(const QString &nameGroup) const
{
    return m_groups[nameGroup];
}

void Dictionary::removeGroup(const QString &nameGroup)
{
    m_groups.remove(nameGroup);
}

void Dictionary::setToGroup(const QString &nameGroup, const DictionaryElement &dictionaryElement)
{
    setToGroup(nameGroup, dictionaryElement.word, dictionaryElement.translate);
}

void Dictionary::setToGroup(const QString &nameGroup, const QString &word,const QString &translate)
{
    if ((word != "") && (translate != "")) {
        m_groups[nameGroup][word] = translate;
    }
}

QString Dictionary::getTranslateFromGroup(const QString &nameGroup, const QString &word) const
{
    return m_groups[nameGroup][word];
}

QString Dictionary::getWordFromGroup(const QString &nameGroup, const QString &translate) const
{
    return  m_groups[nameGroup].key(translate, "");
}

void Dictionary::removeFromGroupWord(const QString &nameGroup, const QString & word)
{
    m_groups[nameGroup].remove(word);
}

const QString keyDict = "dict";
const QString keyGroups = "groups";


QVariantMap getQVariantMap(const QMap<QString, QString> & map)
{
    QVariantMap mapResult;
    foreach (const QString &key, map.keys()) {
        mapResult[key] = map[key];
    }
    return mapResult;
}

QVariantMap getQVariantMap(const QMap<QString, QMap<QString, QString>> & map)
{
    QVariantMap mapResult;
    foreach (const QString &key, map.keys()) {
        mapResult[key] = getQVariantMap(map[key]);
    }
    return mapResult;
}

QMap<QString, QString> getMapOfString(const QVariantMap &map)
{
    QMap<QString, QString> mapResult;
    foreach (const QString &key, map.keys()) {
        mapResult[key] = map[key].toString();
    }
    return mapResult;
}

QMap<QString, QMap<QString, QString>> getMapOfMapOfString(const QVariantMap &map)
{
    QMap<QString, QMap<QString, QString>> mapResult;
    foreach (const QString &key, map.keys()) {
        mapResult[key] = getMapOfString(map[key].toMap());
    }
    return mapResult;
}




QVariant Dictionary::getVariant()
{
    QVariantMap map;

    map[keyDict] = getQVariantMap(m_dict);
    map[keyGroups] = getQVariantMap(m_groups);

    return map;
}

bool Dictionary::setFromVariant(const QVariant &value)
{
    if (value.isNull())
        return false;
    QVariantMap map = value.toMap();
    if (map.count(keyDict) >= 0) {
        m_dict = getMapOfString(map[keyDict].toMap());
    }
    if (map.count(keyGroups) >= 0) {
        m_groups = getMapOfMapOfString(map[keyGroups].toMap());
    }
    return true;

}
