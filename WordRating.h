#ifndef WORDRATING_H
#define WORDRATING_H

#include <QString>
#include <QList>
#include <QVariant>
#include <QVariantMap>

class WordRating
{
public:
    WordRating();
    WordRating(const QString &word, const QString &translate, int limitTrueAnswerWhenFull);
    WordRating(const QVariant &map);

    QString word() const;
    void setWord(const QString &word);

    QString translate() const;
    void setTranslate(const QString &translate);

    int rating() const;
    void setRating(int rating);

    int limitTrueAnswerWhenFull() const;
    void setLimitTrueAnswerWhenFull(int limitTrueAnswerWhenFull);

    QList<bool> historyAnswers() const;
    bool historyAnswer(int index) const;
    int historyAnswersSize() const;

    void setHistoryAnswers(const QList<bool> historyAnswers);
    bool setHistoryAnswer(int index, bool answer);
    void pushHistoryAnswer(bool answer);
    bool removeHistorAnswer(int index);

    void fromVariant(const QVariant & variant);
    QVariant toVariant();

private:
    QString m_word;
    QString m_translate;
    QList<bool> m_historyAnswers;
    qreal m_rating;
    int m_limitTrueAnswerWhenFull;
};

#endif // WORDRATING_H
