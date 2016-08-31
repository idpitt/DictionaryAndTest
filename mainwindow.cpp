#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dictionary.h"
#include "dictionarytest.h"
#include <QMap>
#include <QVector>
#include <QtWidgets>
#include <QVariantMap>
#include <QFile>
#include <QDataStream>
#include <QMetaObject>
#include <QMetaType>
#include <QListWidgetItem>
#include <QComboBox>
#include <QVariant>
#include <QVariantMap>
#include <QVariantList>

#include <JsonTools.h>

const QString strWord = "Слово: ";
const QString strDiv = "; ";
const QString strTranslate = "Перевод: ";
const QString nameSettingsFile = "settings.json";
const QString nameDataFile = "dictionaryData.dat";

const QString keyDictionary = "dictionary";

const QString keyResults = "Results";
const QString keyResultsAnswer = "Answer";
const QString keyResultsIsTrueAnswer = "isTrueAnswer";
const QString keyResultsCountTrueAnswers = "countTrueAnswers";
const QString keyResultsCountAnswers = "countAnswers";





MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    // qRegisterMetaType<Dictionary>("Dictionary");
    ui->setupUi(this);
    connect(ui->pushButtonAddWord, &QPushButton::clicked, this, &MainWindow::addWord);
    connect(ui->pushButtonDeleteWord, &QPushButton::clicked, this, &MainWindow::delWord);
    connect(ui->pushButtonAddGroup, &QPushButton::clicked, this, &MainWindow::addGroup);
    connect(ui->pushButtonDeleteGroup, &QPushButton::clicked, this, &MainWindow::delGroup);
    connect(ui->pushButtonSaveChanges, &QPushButton::clicked, this, &MainWindow::saveChanges);

    connect(ui->pushButtonStartTest, &QPushButton::clicked, this, &MainWindow::startTest);
    connect(ui->pushButtonAnswer, &QPushButton::clicked, this, &MainWindow::answerSelected);
    connect(ui->pushButtonNextQuestion, &QPushButton::clicked, this, &MainWindow::nextQuestion);

    connect(ui->pushButtonEnd, &QPushButton::clicked, this, &MainWindow::endTesting);

    bool b= connect(ui->comboBoxGroups, SIGNAL(currentTextChanged(const QString &)), this, SLOT(groupChanged(const QString &)));
    connect(ui->listWidgetWordTranslate, &QListWidget::itemDoubleClicked, this, &MainWindow::listWordDoubleClicked);
    connect(ui->listWidgetGroup, &QListWidget::itemDoubleClicked, this, &MainWindow::listGroupDoubleClicked);

    QVariant var = JsonTools::fromFile("../statiscticsDict.dat");
    QVariantMap mapToLoadStatistics = var.toMap();
    m_statisticWords = mapFromVariantMap<QVariantMap>(mapToLoadStatistics);


    loadDictionaryData();
    updateDictionaryTest();
    showWordsStatistics();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::addWord()
{
    QString word = ui->lineEditWord->text().toUpper();
    QString translate = ui->lineEditTranslate->text().toUpper();
    addDictionaryElementToArray(DictionaryElement(word, translate));
    updateWordTable();
}

void MainWindow::delWord()
{
    int index = ui->listWidgetWordTranslate->currentRow();
    if (index < 0) {
        return;
    }
    QString message = tr("Вы уверены что хотите удалить слово: ") + m_elements[index].word;
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, tr("Удаление слова"),
                                  (message),
                                  QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        delDictionaryElementFromArray(index);
        updateWordTable();
    }
}

void MainWindow::addGroup()
{

    QString text = ui->lineEditGroup->text();
    if (text == "")
        return;
    QMap<QString, QString> map;
    m_dictionary.setGroup(ui->lineEditGroup->text(), map);
    updateGroupTable();
}

void MainWindow::delGroup()
{
    m_dictionary.removeGroup(m_currentGroup);
    updateGroupTable();
}

void MainWindow:: saveChanges()
{
    saveDictionaryData();
    updateDictionaryTest();
}


void MainWindow::updateWordTable()
{
    ui->listWidgetWordTranslate->clear();
    foreach (const DictionaryElement & element, m_elements) {
        ui->listWidgetWordTranslate->addItem(getListItemDictionaryElement(element.word, element.translate));
    }
}

void MainWindow::updateGroupTable()
{
    ui->comboBoxGroups->clear();
    QStringList list = m_dictionary.getGroups().keys();
    ui->comboBoxGroups->addItems(list);
}

void MainWindow::updateGroupWordTable()
{

    //    QMap<QString, QString> mapWordOfGroup = m_dictionary.getGroup(m_currentGroup);
    //    QStringList listWord = mapWordOfGroup.keys();

    //    foreach (const QString &word, listWord) {
    //        ui->listWidgetGroup->addItem(getListItemDictionaryElement(word, mapWordOfGroup[word]));
    //    }
    // m_elementsOfGroup
    ui->listWidgetGroup->clear();
    m_elementsOfGroup[m_currentGroup].clear();
    QMap<QString, QString> mapWordOfGroup = m_dictionary.getGroup(m_currentGroup);
    QStringList listWord = mapWordOfGroup.keys();

    foreach (const QString &word, listWord) {
        //        ui->listWidgetGroup->addItem(getListItemDictionaryElement(word, mapWordOfGroup[word]));
        m_elementsOfGroup[m_currentGroup].push_back(DictionaryElement(word, mapWordOfGroup[word]));
    }

    foreach (const DictionaryElement & element, m_elementsOfGroup[m_currentGroup]) {
        ui->listWidgetGroup->addItem(getListItemDictionaryElement(element.word, element.translate));
    }
}

void MainWindow::groupChanged(const QString & group)
{
    m_currentGroup = group;
    updateGroupWordTable();
}


void MainWindow::listWordDoubleClicked(QListWidgetItem *item)
{
    if (m_currentGroup == "")
        return;

    int index = ui->listWidgetWordTranslate->currentIndex().row();
    addDictionaryElementToGroup(DictionaryElement(m_elements[index].word, m_elements[index].translate), m_currentGroup);


    updateGroupWordTable();
}

void MainWindow::listGroupDoubleClicked(QListWidgetItem *item)
{
    if (m_currentGroup == "")
        return;
    int index = ui->listWidgetGroup->currentIndex().row();
    if (m_elementsOfGroup[m_currentGroup].size() <= index)
        return;
    removeDictionaryElementFromGroup(m_elementsOfGroup[m_currentGroup][index], m_currentGroup);


    //    QString nameGroup;
    //    m_dictionary.setToGroup(m_currentGroup, m_elements[index].word, m_elements[index].translate);
    updateGroupWordTable();
}


QString MainWindow::getListItemDictionaryElement(const QString &word, const QString &translate)
{
    return strWord+word+strDiv+strTranslate+translate;
}

void MainWindow::addDictionaryElementToArray(const DictionaryElement &element)
{
    m_dictionary.setWord(element.word, element.translate);
    for (int i = 0; i < m_elements.size(); ++i) {
        if (m_elements[i].word == element.word) {
            m_elements[i].translate = element.translate;
            return;
        }
    }
    m_elements.push_back(element);
}

void MainWindow::delDictionaryElementFromArray(int index)
{
    m_dictionary.removeWord(m_elements[index].word);
    m_elements.remove(index);
}

void MainWindow::updateListDictionary()
{
    m_elements.clear();
    foreach (const QString &word, m_dictionary.getDict().keys()) {
        m_elements.push_back(DictionaryElement(word, m_dictionary.getDict()[word]));
    }
    updateWordTable();
}


void MainWindow::updateDictionaryTest()
{
    m_dictionaryTest.setDictionary(m_dictionary);
}

////

void MainWindow::saveSettings()
{


}

void MainWindow::loadSettings()
{

}


void MainWindow::saveDictionaryData()
{
    QFile file(nameDataFile);
    if (!file.open(QIODevice::WriteOnly) ) {
        qDebug() << "not Open";
    }

    QDataStream out(&file);
    QVariantMap saveMap;
    //  QVariant variant = QVariant::fromValue(m_dictionary);

    saveMap[keyDictionary] = m_dictionary.getVariant();
    out << saveMap;

    file.close();
}

void MainWindow::loadDictionaryData()
{
    QFile file(nameDataFile);

    if (!file.open(QIODevice::ReadWrite) ) {
        qDebug() << "not Open";
    }

    QDataStream inp(&file);
    QVariantMap loadMap;
    inp >> loadMap;

    if (loadMap.count(keyDictionary) > 0) {
        m_dictionary.setFromVariant(loadMap[keyDictionary]);
    }
    updateListDictionary();
    updateGroupTable();
    updateGroupsForTest();
}
//
void MainWindow::updateGroupsForTest()
{
    ui->comboBoxGroupToTest->clear();
    ui->comboBoxGroupToTest->addItem("ВСЕ");
    QStringList list = m_dictionary.getGroups().keys();
    ui->comboBoxGroupToTest->addItems(list);
}

//
void MainWindow::startTest()
{
    std::srand(QTime(0,0,0).secsTo(QTime::currentTime()));
    m_resultAnswers.clear();
    m_currentIndexTest = 0;
    m_currentResult.clear();
    m_currentResult[keyResults] = QVariantList();
    ui->textBrowserResuslt->clear();
    m_isQuestionTranslate = ui->radioButtonEnterTranslate->isChecked();
    if (ui->comboBoxGroupToTest->currentIndex() == 0)
        m_vectorForTest = m_dictionaryTest.getArrayForTest();
    else
        m_vectorForTest = m_dictionaryTest.getArrayForTest(ui->comboBoxGroupToTest->currentText());
    foreach (const DictionaryElement &dictEl, m_vectorForTest) {
        m_resultAnswers.push_back(Answers(dictEl.word, dictEl.translate));
    }

    ui->labelCount->setText(QString::number(m_vectorForTest.size() ) );
    ui->labelStateTest->setText(tr("Тестирование в процессе."));
    ui->labelStateAnswer->setText("");
    ui->labelWordOrTranslate->setText(m_isQuestionTranslate ?
                                          tr("Cлово: ") : tr("Перевод: "));
    ui->labelEnterWordOrTranslate->setText(m_isQuestionTranslate ?
                                               tr("Введите перевод:") : tr("Введите слово:"));
    ui->pushButtonNextQuestion->setVisible(false);
    showQuestion();

}

void MainWindow::answerSelected()
{

    if (m_vectorForTest.size() <= 0)
        return;
    bool isTrueAnswer = checkQuestion();
    m_resultAnswers[m_currentIndexTest].answers.push_back(ui->lineEditWordOrTranslate->text());
    QVariantMap mapAnswer;
    mapAnswer[keyResultsIsTrueAnswer] = isTrueAnswer;
    QVariantList tempList= m_currentResult[keyResults].toList();
    if (tempList.size() > m_currentIndexTest) {
        tempList[m_currentIndexTest] = mapAnswer;
    }
    else  {
        tempList.push_back(mapAnswer);
    }
    m_currentResult[keyResults]= tempList;

    QString strIsCorrectlyAnswer = isTrueAnswer
            ? tr("правильный") : tr("неправильный");
    QString strStateOfAnswer = tr("На вопрос №")
            + QString::number(m_currentIndexTest + 1)+ tr(" дан ")
            + strIsCorrectlyAnswer + " ответ.\n";
    if (!isTrueAnswer) {
        ui->pushButtonNextQuestion->setVisible(true);
    }
    else {
        QString strToAdd = "Правильный ответ: "
                +(m_isQuestionTranslate
                  ? m_vectorForTest[m_currentIndexTest].translate.toUpper()
                  : m_vectorForTest[m_currentIndexTest].word.toUpper());
        strStateOfAnswer+=strToAdd;
        ui->pushButtonNextQuestion->setVisible(false);
    }
    ui->labelStateAnswer->setText(strStateOfAnswer);
    m_counterAnswer++;
    if (isTrueAnswer) {
        addTrueAnswerInWordStatistick(m_vectorForTest[m_currentIndexTest].word);
        m_currentIndexTest++;
        showQuestion();
    }

}

void MainWindow::showQuestion()
{
    m_counterAnswer = 0;
    ui->lineEditWordOrTranslate->clear();
    if (m_currentIndexTest < m_vectorForTest.size()) {
        ui->labelWordOrTranslateValue->setText(m_isQuestionTranslate ?
                                                   m_vectorForTest[m_currentIndexTest].word :
                                                   m_vectorForTest[m_currentIndexTest].translate);
        ui->labelCurrentNum->setText(QString::number(m_currentIndexTest + 1));
    }
    else {
        endTesting();
    }
}

bool MainWindow::checkQuestion()
{
    QString strAnswer = ui->lineEditWordOrTranslate->text().toUpper();

    if (((m_isQuestionTranslate)&&(m_vectorForTest[m_currentIndexTest].translate.toUpper() == strAnswer) ) ||
            ((!m_isQuestionTranslate)&&(m_vectorForTest[m_currentIndexTest].word.toUpper() == strAnswer) )   ) {
        return true;
    }

    return false;
}

void MainWindow::nextQuestion()
{
    ui->pushButtonNextQuestion->setVisible(false);
    QString strIsCorrectlyAnswer = tr("неправильный");
    QString strStateOfAnswer = tr("На вопрос №")
            + QString::number(m_currentIndexTest + 1)+ tr(" дан ")
            + strIsCorrectlyAnswer + " ответ.\n";
    QString strToAdd = "Правильный ответ: "
            +(m_isQuestionTranslate
              ? m_vectorForTest[m_currentIndexTest].translate.toUpper()
              : m_vectorForTest[m_currentIndexTest].word.toUpper());
    addAnswerInWordStatistick(m_vectorForTest[m_currentIndexTest].word);
    m_currentIndexTest++;

    strStateOfAnswer+=strToAdd;
    ui->labelStateAnswer->setText(strStateOfAnswer);

    showQuestion();

}

void MainWindow::endTesting()
{
    if (m_currentIndexTest < m_vectorForTest.size()) {
        addAnswerInWordStatistick(m_vectorForTest[m_currentIndexTest].word);
    }
    m_currentResult[keyResultsCountTrueAnswers] = 0;
    QVariantList listAnswers = m_currentResult[keyResults].toList();
    foreach (const QVariant &result, listAnswers) {
        QVariantMap mapAnswer = result.toMap();
        if (mapAnswer[keyResultsIsTrueAnswer].toBool() ) {
            int countTrueAnswers = m_currentResult[keyResultsCountTrueAnswers].toInt();
            m_currentResult[keyResultsCountTrueAnswers] = countTrueAnswers + 1;
        }
    }

    ui->labelStateTest->setText(tr("Тестирование окончено."));
    showResult();
    showWordsStatistics();

}

void MainWindow::showResult()
{
    ui->textBrowserResuslt->clear();
    QString str;
    int countAnswers = m_currentResult[keyResults].toList().size();
    str = "Колличество правильных ответов: " + QString::number(m_currentResult[keyResultsCountTrueAnswers].toInt())
            + " из " + QString::number(countAnswers) ;
    ui->textBrowserResuslt->append(str);
    ui->textBrowserResuslt->append("");
    for (int i = 0; i < m_resultAnswers.size(); ++i) {
        str = QString::number(i+1) + ": ";
        str += m_isQuestionTranslate ? ("Слово: " + m_resultAnswers[i].word)
                                     : ("Перевод: " + m_resultAnswers[i].translate);
        ui->textBrowserResuslt->append(str);
        str = "Ответы: ";
        for (int j = 0; j < m_resultAnswers[i].answers.size(); ++j) {
            if (j != 0)
                str+=", ";
            str+=m_resultAnswers[i].answers[j];
        }
        ui->textBrowserResuslt->append(str);
        ui->textBrowserResuslt->append("");
    }

}

void MainWindow::showWordsStatistics()
{
    ui->textBrowserStatistics->clear();
    foreach (const QString &key, m_statisticWords.keys()) {
        QString strStatistics = "Слово: ";
        strStatistics += key;
        ui->textBrowserStatistics->append(strStatistics);
        strStatistics = " Всего ответов: "
                + QString::number(m_statisticWords[key][statisticsKeys::countAnswer].toFloat()) + "; "
                + "Правильных ответов: "
                + QString::number(m_statisticWords[key][statisticsKeys::countTrueAnswer].toFloat());
        ui->textBrowserStatistics->append(strStatistics);
        ui->textBrowserStatistics->append("");
    }
    QVariantMap mapToSaveStatistics = mapToVariantMap<QVariantMap>(m_statisticWords);
    JsonTools::toFile(mapToSaveStatistics, "../statiscticsDict.dat");
}

//


void MainWindow::addTrueAnswerInWordStatistick(const QString & word)
{
    if (m_statisticWords.count(word) <= 0) {
        m_statisticWords[word] = QVariantMap();
        m_statisticWords[word][statisticsKeys::countTrueAnswer] = 0.0;
    }
    else {
        if (m_statisticWords[word].count(statisticsKeys::countTrueAnswer) <= 0) {
            m_statisticWords[word][statisticsKeys::countTrueAnswer] = 0.0;
        }
    }
    m_statisticWords[word][statisticsKeys::countTrueAnswer] =
            m_statisticWords[word][statisticsKeys::countTrueAnswer].toFloat() + 1;
    addAnswerInWordStatistick(word);
}

void MainWindow::addAnswerInWordStatistick(const QString & word)
{
    if (m_statisticWords.count(word) <= 0) {
        m_statisticWords[word] = QVariantMap();
        m_statisticWords[word][statisticsKeys::countAnswer] = 0.0;
    }
    else {
        if (m_statisticWords[word].count(statisticsKeys::countAnswer) <= 0) {
            m_statisticWords[word][statisticsKeys::countAnswer] = 0.0;
        }
    }
    m_statisticWords[word][statisticsKeys::countAnswer] =
            m_statisticWords[word][statisticsKeys::countAnswer].toFloat() + 1;
}


void MainWindow::updateStatistickToWord(const QString &word, const QVariantMap &stgAnswer)
{

}

void MainWindow::addDictionaryElementToGroup(const DictionaryElement &element, const QString &nameGroup)
{
    //   m_elementsOfGroup[nameGroup].push_back(element);
    m_dictionary.setToGroup(nameGroup, element.word, element.translate);
}

void MainWindow::removeDictionaryElementFromGroup(const DictionaryElement &element, const QString &nameGroup)
{
    m_elementsOfGroup[nameGroup].removeOne(element);
    m_dictionary.removeFromGroupWord(nameGroup, element.word);
}

///////////////////////////////////////
bool checkExistsSettingsFile() {
    QFile file(nameSettingsFile);
    if (file.exists()) {
        return true;
    }
    return false;
}
