#ifndef HOTKEYFILTER_H
#define HOTKEYFILTER_H
#pragma once

#include <QObject>
#include <QAbstractNativeEventFilter>
#include <Windows.h>

class HotkeyFilter : public QObject, public QAbstractNativeEventFilter
{
    Q_OBJECT

public:
    explicit HotkeyFilter(QObject *parent = nullptr);
    ~HotkeyFilter();

    bool nativeEventFilter(const QByteArray &eventType, void *message, qintptr *result) override;

signals:
    void hotkeyPressed();
};


#endif // HOTKEYFILTER_H
