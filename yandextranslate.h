#ifndef YANDEXTRANSLATE_H
#define YANDEXTRANSLATE_H
#pragma once

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class YandexTranslate : public QObject
{
    Q_OBJECT

public:
    explicit YandexTranslate(QObject *parent = nullptr);
    void translate(const QString &text, const QString &fromLang, const QString &toLang);

signals:
    void translationFinished(const QString &translatedText);
    void translationError(const QString &errorMessage);

private slots:
    void handleReply(QNetworkReply *reply);

private:
    QNetworkAccessManager networkManager;
    const QString apiKey = "API-key-translate-Yandex";
};

#endif // YANDEXTRANSLATE_H
