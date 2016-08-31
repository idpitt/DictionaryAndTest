#include "WordRating.h"
#include "HelpfulFuncsToContainters.h"

namespace WordRatinKeys {
const QString word = "word";
const QString translate = "translate";
const QString historyAnswers = "historyAnswers";
const QString rating = "rating";
const QString limitTruAnswerWhenFull = "limitTruAnswerWhenFull";
}

WordRating::WordRating()
{

}

WordRating::WordRating(const QVariant &map)
{
    fromVariant(map);
}


WordRating::WordRating(const QString &word, const QString &translate, int limitTrueAnswerWhenFull)
{
    m_word = word;
    m_translate = translate;
    m_limitTrueAnswerWhenFull = limitTrueAnswerWhenFull;
}

QString WordRating::word() const
{
    return m_word;
}

void WordRating::setWord(const QString &word)
{
    m_word = word;
}

QString WordRating::translate() const
{
    return m_translate;
}

void WordRating::setTranslate(const QString &translate)
{
    m_translate = translate;
}

int WordRating::rating() const
{
    return m_rating;
}

void WordRating::setRating(int rating)
{
    m_rating = rating;
}

int WordRating::limitTrueAnswerWhenFull() const
{
    return m_limitTrueAnswerWhenFull;
}

void WordRating::setLimitTrueAnswerWhenFull(int limitTrueAnswerWhenFull)
{
    m_limitTrueAnswerWhenFull = limitTrueAnswerWhenFull;
}

QList<bool> WordRating::historyAnswers() const
{
    return m_historyAnswers;
}

bool WordRating::historyAnswer(int index) const
{
    return m_historyAnswers[index];
}

int WordRating::historyAnswersSize() const
{
    return m_historyAnswers.size();
}

void WordRating::setHistoryAnswers(const QList<bool> historyAnswers)
{
    m_historyAnswers = historyAnswers;
}

bool WordRating::setHistoryAnswer(int index, bool answer)
{
    if (index < m_historyAnswers.size()) {
        m_historyAnswers[index] = answer;
        return true;
    }
    else if (index < m_historyAnswers.size()) {
        m_historyAnswers.push_back(answer);
        return true;
    }
    return false;
}

void WordRating::pushHistoryAnswer(bool answer)
{
    m_historyAnswers.push_back(answer);
}

bool WordRating::removeHistorAnswer(int index)
{
    if (index < m_historyAnswers.size()) {
        m_historyAnswers.removeAt(index);
        return true;
    }
    else {
        return false;
    }
}

void WordRating::fromVariant(const QVariant & variant)
{
    QVariantMap map = variant.toMap();

    m_word = map[WordRatinKeys::word].toString();
    m_translate = map[WordRatinKeys::translate].toString();
    m_historyAnswers = variantListToTypeList<bool>(map[WordRatinKeys::historyAnswers].toList());
    m_rating = map[WordRatinKeys::rating].toFloat();
    m_limitTrueAnswerWhenFull = map[WordRatinKeys::limitTruAnswerWhenFull].toInt();

}

QVariant WordRating::toVariant()
{
    QVariantMap map;
    map[WordRatinKeys::word] = m_word;
    map[WordRatinKeys::translate] = m_translate;
    map[WordRatinKeys::historyAnswers] = TypeListToVariantList<bool>(m_historyAnswers);
    map[WordRatinKeys::rating] = m_rating;
    map[WordRatinKeys::limitTruAnswerWhenFull] = m_limitTrueAnswerWhenFull;

    return map;
}
