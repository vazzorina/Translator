QT += core gui widgets network
TEMPLATE = app
TARGET = Translate

SOURCES += main.cpp \
           mainwindow.cpp \
           hotkeyfilter.cpp \
           yandextranslate.cpp

HEADERS += mainwindow.h \
           hotkeyfilter.h \
           yandextranslate.h

# Подключаем user32 для Windows API
win32:LIBS += -luser32

STATECHARTS +=

RESOURCES += \
    resources.qrc
