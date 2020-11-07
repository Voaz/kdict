#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include <QObject>
#include <QString>
#include <qqml.h>
#include <QNetworkAccessManager>
#include <QTimer>


class Translator : public QObject {
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(QString targetWord READ targetWord WRITE setTargetWord NOTIFY targetWordChanged)
    Q_PROPERTY(QString targetLang READ targetLang WRITE setTargetLang NOTIFY targetLangChanged)

    Q_PROPERTY(QString sourceWord READ sourceWord WRITE setSourceWord NOTIFY sourceWordChanged)
    Q_PROPERTY(QString sourceLang READ sourceLang WRITE setSourceLang NOTIFY sourceLangChanged)

public:
    explicit Translator(QObject *parent = nullptr);

    QString targetWord() const;
    QString targetLang() const;

    QString sourceWord() const;
    QString sourceLang() const;


public slots:
    void setTargetWord(QString targetWord);
    void setTargetLang(QString targetLang);

    void setSourceWord(QString sourceWord);
    void setSourceLang(QString sourceLang);


signals:
    void targetWordChanged(QString targetWord);
    void targetLangChanged(QString targetLang);

    void sourceWordChanged(QString sourceWord);
    void sourceLangChanged(QString sourceLang);


private slots:
    void updateValues(QNetworkReply *data);

private:
    void requestData();
    QString loadWord();

    QString m_targetWord;
    QString m_targetLang;
    QString m_sourceWord;
    QString m_sourceLang;
    QNetworkAccessManager accMan;
    QTimer m_timer;

    enum class ApiType {
        GOOGLE,
        DICTIONARYAPI
    } m_apiType;
};

#endif // TRANSLATOR_H
