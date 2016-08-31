#ifndef DICTIONARYTEST_H
#define DICTIONARYTEST_H

#include "dictionary.h"

class DictionaryTest
{
public:
    DictionaryTest();

    void setDictionary(const Dictionary &dict);
    Dictionary getDictionary() const;

    QVector<DictionaryElement> getArrayForTest() const;
    QVector<DictionaryElement> getArrayForTest(const QString &nameGroup) const;

private:
    Dictionary m_dict;

    QVector<DictionaryElement> createArrayForTest(const QMap<QString, QString> &mapDict) const;
};

#endif // DICTIONARYTEST_H
