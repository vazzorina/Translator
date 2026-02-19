#include <QApplication>
#include <QClipboard>
#include <QKeyEvent>
#include <QTimer>
#include <QMimeData>

#include "mainwindow.h"
#include "hotkeyfilter.h"
#include "yandextranslate.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow w;
    w.hide();

    HotkeyFilter *filter = new HotkeyFilter(&app);
    QClipboard *clipboard = QApplication::clipboard();

    YandexTranslate *yandexTranslate = new YandexTranslate();
    QObject::connect(yandexTranslate, &YandexTranslate::translationFinished, &w, [&](const QString &translatedText) {
        w.appendText(translatedText);
    });
    QObject::connect(yandexTranslate, &YandexTranslate::translationError, &w, [&](const QString &errorMessage) {
        w.appendText(QString("Ошибка перевода: %1").arg(errorMessage));
    });

    QObject::connect(filter, &HotkeyFilter::hotkeyPressed, [&]() {
        w.clearText();
        clipboard->clear();
        keybd_event(VK_CONTROL, 0, 0, 0);
        keybd_event(0x43, 0, 0, 0);
        keybd_event(0x43, 0, KEYEVENTF_KEYUP, 0);
        keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, 0);

        QTimer::singleShot(200, [&]() {
            const QMimeData *mimeData = clipboard->mimeData();
            if (mimeData && mimeData->hasText()) {
                QString selectedText = mimeData->text();

                QString fromLang = "en";
                QString toLang = "ru";

                yandexTranslate->translate(selectedText, fromLang, toLang);
            } else {
                w.appendText("Ошибка: Не удалось получить текст. Возможно, текст не был выделен.");
            }
            w.showNearSelectedText();
        });
    });

    app.installNativeEventFilter(filter);
    return app.exec();
}
