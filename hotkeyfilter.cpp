#include "hotkeyfilter.h"

#define HOTKEY_ID 1

HotkeyFilter::HotkeyFilter(QObject *parent) : QObject(parent)
{
    RegisterHotKey(nullptr, HOTKEY_ID, MOD_CONTROL, 0xC0);
}

HotkeyFilter::~HotkeyFilter()
{
    UnregisterHotKey(nullptr, HOTKEY_ID);
}

bool HotkeyFilter::nativeEventFilter(const QByteArray &eventType, void *message, qintptr *result)
{
    if (eventType == "windows_generic_MSG") {
        MSG *msg = static_cast<MSG *>(message);
        if (msg->message == WM_HOTKEY && msg->wParam == HOTKEY_ID) {
            emit hotkeyPressed();
            return true;
        }
    }
    return false;
}
