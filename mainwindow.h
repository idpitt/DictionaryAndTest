#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "dictionary.h"
#include "dictionarytest.h"

//class Dictionary;
//class DictionaryTest;
//struct DictionaryElement;
class QListWidgetItem;

namespace Ui {
class MainWindow;
}

namespace statisticsKeys {
    const QString countTrueAnswer = "countTrueAnswer";
    const QString countAnswer = "countAnswer";
    const QString totalStudied = "totalStudied";
}

struct Answers {
    Answers () { }
    Answers (const QString &_word, const QString &_translate) :
    word(_word), translate(_translate) { }
    QString word;
    QString translate;
    QList<QString> answers;
};

template <typename T>QVariantMap mapToVariantMap(const QMap<QString, T>& map) {
    QVariantMap resultMap;
    foreach (const QString &key, map.keys()) {
        resultMap[key] = QVariant::fromValue(map[key]);
    }

    return resultMap;
}

template <typename T>QMap<QString, T> mapFromVariantMap(const QVariantMap& map) {
    QMap<QString, T> resultMap;
    foreach (const QString &key, map.keys()) {
        resultMap[key] = qvariant_cast<T>(map[key]);
    }

    return resultMap;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    void addWord();
    void delWord();
    void addGroup();
    void delGroup();
    void saveChanges();


    void updateWordTable();
    void updateGroupTable();
    void updateGroupWordTable();

    void groupChanged(const QString & group);

    void listWordDoubleClicked(QListWidgetItem *item);
    void listGroupDoubleClicked(QListWidgetItem *item);



private:
    Ui::MainWindow *ui;

    Dictionary m_dictionary;
    DictionaryTest m_dictionaryTest;

    QVector<DictionaryElement> m_elements;
    QVector<DictionaryElement> m_vectorForTest;
    QMap<QString, QVector<DictionaryElement>> m_elementsOfGroup;
    QMap<QString, QVariantMap> m_statisticWords;
    QList<Answers> m_resultAnswers;

    QVariantMap m_currentResult;
    QString m_currentGroup;
    int m_currentIndexTest;
    bool m_isQuestionTranslate;
    int m_counterAnswer;

    QString getListItemDictionaryElement(const QString &word, const QString &translate);
    void addDictionaryElementToArray(const DictionaryElement &element);
    void updateListDictionary();

    void delDictionaryElementFromArray(int index);

    void updateDictionaryTest();
    void saveSettings();
    void loadSettings();
    void saveDictionaryData();
    void loadDictionaryData();
    //
    void updateGroupsForTest();

    //
    void startTest();
    void answerSelected();
    void showQuestion();
    bool checkQuestion();
    void nextQuestion();
    void endTesting();
    void showResult();
    void showWordsStatistics();
    //
    void addTrueAnswerInWordStatistick(const QString & word);
    void addAnswerInWordStatistick(const QString & word);

    void updateStatistickToWord(const QString &word, const QVariantMap &stgAnswer);

    void addDictionaryElementToGroup(const DictionaryElement &element, const QString &nameGroup);
    void removeDictionaryElementFromGroup(const DictionaryElement &element, const QString &nameGroup);

};

#endif // MAINWINDOW_H
