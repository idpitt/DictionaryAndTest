#include "dictionarytest.h"
#include "dictionary.h"
#include <QVector>

DictionaryTest::DictionaryTest()
{


}

void DictionaryTest::setDictionary(const Dictionary &dict)
{
    m_dict = dict;
}

Dictionary DictionaryTest::getDictionary() const
{
    return m_dict;
}

QVector<DictionaryElement> DictionaryTest::getArrayForTest() const
{
    return createArrayForTest(m_dict.getDict());
}

QVector<DictionaryElement> DictionaryTest::getArrayForTest(const QString &nameGroup) const
{
    return createArrayForTest(m_dict.getGroup(nameGroup));
}

//
QVector<DictionaryElement> DictionaryTest::createArrayForTest(const QMap<QString, QString> &mapDict) const
{
    QVector<DictionaryElement> vector;

    foreach (const QString &word, mapDict.keys()) {
        vector.push_back(DictionaryElement(word, mapDict[word]));
    }

   std::random_shuffle ( vector.begin(), vector.end() );
    return vector;
}
