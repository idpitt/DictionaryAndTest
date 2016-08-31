#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <QMap>
#include <QString>
#include <QObject>
#include <QVariant>



class DictionaryElement {
public:
    DictionaryElement() {}
    DictionaryElement::DictionaryElement (const QString &r_word, const QString &r_translate);
    QString word;
    QString translate;
    friend bool operator ==(const DictionaryElement &element1, const DictionaryElement &element2 ); //перегрузка ==

};

bool operator == ( const DictionaryElement &element1, const DictionaryElement &element2 );


class Dictionary
{

public:
    Dictionary::Dictionary();

    void setDict(const QMap<QString, QString> &dict);
    QMap<QString, QString> getDict() const;
    bool setWord(const QString &word, const QString &translate);
    QString getWord(const QString &translate) const;
    QString getTranslate(const QString &word) const;

    void removeWord(const QString &word);

    void setGroups(const QMap<QString, QMap<QString, QString>> &groups);
    QMap<QString, QMap<QString, QString>> getGroups() const;

    void setGroup(const QString &nameGroup, const QMap<QString, QString> &groupDict);
    QMap<QString, QString> getGroup(const QString &nameGroup) const;
    void removeGroup(const QString &nameGroup);

    void setToGroup(const QString &nameGroup, const DictionaryElement &dictionaryElement);
    void setToGroup(const QString &nameGroup, const QString &word,const QString &translate);
    QString getTranslateFromGroup(const QString &nameGroup, const QString &word) const;
    QString getWordFromGroup(const QString &nameGroup, const QString &translate) const;

    void removeFromGroupWord(const QString &nameGroup, const QString & word);

    QVariant getVariant();
    bool  setFromVariant(const QVariant &value);

private:
    QMap<QString, QString> m_dict;
    QMap<QString, QMap<QString, QString>> m_groups;

};
Q_DECLARE_METATYPE(Dictionary)

#endif // DICTIONARY_H
