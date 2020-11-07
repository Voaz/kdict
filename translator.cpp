
#include "translator.h"
#include <QNetworkReply>

#include <QJsonDocument>
#include <QJsonArray>
#include <QCborValue>
#include <QDebug>
#include <QProcess>
#include <QJsonObject>

Translator::Translator(QObject *parent) : QObject(parent)
{
    connect(&accMan, &QNetworkAccessManager::finished, this, &Translator::updateValues);

    connect(&m_timer, &QTimer::timeout, [this] {
        QString tWord = loadWord();
        if (tWord != sourceWord()) {

            accMan.get(QNetworkRequest(QUrl("https://api.dictionaryapi.dev/api/v2/entries/en/" + tWord)));
//            accMan.get(QNetworkRequest(QUrl("https://translate.googleapis.com/translate_a/single?client=gtx&sl=auto&tl=ru&dt=t&q=" + tWord)));
        }
    });
    m_timer.setInterval(5000);
    m_timer.setSingleShot(false);
    m_timer.start();

    m_apiType = ApiType::DICTIONARYAPI;
}

QString Translator::targetWord() const
{
    return m_targetWord;
}

QString Translator::targetLang() const
{
    return m_targetLang;
}

QString Translator::sourceLang() const
{
    return m_sourceLang;
}

QString Translator::sourceWord() const
{
    return m_sourceWord;
}

void Translator::setTargetWord(QString targetWord)
{
    if (m_targetWord == targetWord)
        return;

    m_targetWord = targetWord;
    emit targetWordChanged(m_targetWord);
}

void Translator::setTargetLang(QString targetLang)
{
    if (m_targetLang == targetLang)
        return;

    m_targetLang = targetLang;
    emit targetLangChanged(m_targetLang);
}

void Translator::setSourceWord(QString sourceWord)
{
    if (m_sourceWord == sourceWord)
        return;

    m_sourceWord = sourceWord;
    emit sourceWordChanged(m_sourceWord);
}

void Translator::setSourceLang(QString sourceLang)
{
    if (m_sourceLang == sourceLang)
        return;

    m_sourceLang = sourceLang;
    emit sourceLangChanged(m_sourceLang);
}

void Translator::requestData() {

}

void Translator::updateValues(QNetworkReply *data) {
    auto d = data->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(d);
    switch (m_apiType) {
        case ApiType::GOOGLE: {
            if (doc.isArray()) {
                QJsonArray a = doc.array();
                if (a[0].isArray()) {
                    QJsonArray sub_a = a[0].toArray();
                    if (sub_a[0].isArray()) {
                        QJsonArray subSub_a = sub_a[0].toArray();
                        if (subSub_a[0].isString()) {
                            setTargetWord(subSub_a[0].toString());
                            setSourceWord(subSub_a[1].toString());
                            qDebug() << "string is set";
                        }
                    } else {
                        qDebug() << "data corrupt";
                    }
                } else {
                    qDebug() << "data corrupt";
                }
            } else {
                qDebug() << "data corrupt: " << d;
            }
            break;
        }
        case ApiType::DICTIONARYAPI: {
            if (doc.isArray()) {
                QJsonArray a = doc.array();
                if (a[0].isObject()) {
                    QJsonObject subSub_a = a[0].toObject();
                    if (subSub_a["word"].isString()) {
                        setSourceWord(subSub_a["word"].toString());
                        qDebug() << "target word is set";
                    }
                    if (subSub_a["meanings"].isArray()) {
                        QJsonArray meaningArray = subSub_a["meanings"].toArray();
                        if (meaningArray[0].isObject()) {
                            QJsonObject meaningObj = meaningArray[0].toObject();
                            if (meaningObj["partOfSpeech"].isString()) {
                                setTargetWord(meaningObj["partOfSpeech"].toString());
                                setTargetWord(targetWord() + "\n");
                                QString val = "def: " + meaningObj["definitions"].toArray()[0].toObject()["definition"].toString();
                                val += "\nExmpl: ";
                                val += meaningObj["definitions"].toArray()[0].toObject()["example"].toString();
                                setTargetWord(targetWord() + val);
                            }
                        }
                    }
                } else {
                    qDebug() << "data corrupt";
                }
            } else {
                qDebug() << "data corrupt" << d;
            }

            break;
        }
    }
    data->deleteLater();
}

QString Translator::loadWord() {
    QString val = "";
    QProcess proc;
    proc.start(QString("xsel"), QStringList());
    if (!proc.waitForStarted(2000)) //default wait time 30 sec
        qWarning() << " cannot start process ";

    if (!proc.waitForFinished(2000))
        qWarning() << " cannot finish process ";

    val = QString::fromLocal8Bit(proc.readAll());
    qDebug() << val;
    return val;
}
