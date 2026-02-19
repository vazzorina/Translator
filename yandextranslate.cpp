#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QNetworkRequest>
#include <QNetworkReply>

#include "yandextranslate.h"

YandexTranslate::YandexTranslate(QObject *parent) : QObject(parent)
{
    connect(&networkManager, &QNetworkAccessManager::finished,
            this, &YandexTranslate::handleReply);
}

void YandexTranslate::translate(const QString &text, const QString &fromLang, const QString &toLang)
{
    QUrl url("https://translate.api.cloud.yandex.net/translate/v2/translate");
    QNetworkRequest request(url);

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", "Api-Key " + apiKey.toUtf8());

    request.setAttribute(QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::AlwaysNetwork);
    request.setAttribute(QNetworkRequest::CacheSaveControlAttribute, false);

    QJsonObject json;
    json["sourceLanguageCode"] = fromLang.toLower();
    json["targetLanguageCode"] = toLang.toLower();
    json["format"] = "PLAIN_TEXT";
    json["speller"] = false;

    QJsonArray texts;
    texts.append(text);
    json["texts"] = texts;

    QJsonDocument doc(json);
    QByteArray data = doc.toJson();

    networkManager.post(request, data);
}

void YandexTranslate::handleReply(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray response = reply->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(response);
        if (doc.isObject()) {
            QJsonObject obj = doc.object();
            QJsonArray translations = obj["translations"].toArray();
            if (!translations.isEmpty()) {
                QString translatedText = translations[0].toObject()["text"].toString();
                emit translationFinished(translatedText);
            } else {
                emit translationError("Empty translation response.");
            }
        } else {
            emit translationError("Invalid JSON response.");
        }
    } else {
        emit translationError(reply->errorString());
    }
    reply->deleteLater();
}
