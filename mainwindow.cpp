#include "mainwindow.h"
#include <QSystemTrayIcon>
#include <QMenu>
#include <QCloseEvent>
#include <QAction>
#include <QApplication>
#include <QGuiApplication>
#include <QScreen>
#include <QCursor>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Popup);
    setAttribute(Qt::WA_TranslucentBackground);

    QWidget *central = new QWidget(this);
    central->setObjectName("centralWidget");
    central->setStyleSheet(R"(
        #centralWidget {
            background-color: #ebebd3;
            border-radius: 10px;
            border: 2px solid #e9ecef;
        }
    )");
    setFixedHeight(MIN_HEIGHT);
    setFixedWidth(MIN_WIDTH);

    QVBoxLayout *layout = new QVBoxLayout(central);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    textOutput = new QTextEdit(this);
    textOutput->setReadOnly(true);
    textOutput->setPlaceholderText("Загрузка...");
    textOutput->setWordWrapMode(QTextOption::WordWrap);
    textOutput->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    textOutput->setStyleSheet(R"(
        QTextEdit {
            background-color: #ebebd3;
            border: 2px solid #f4d35e;
            color: #343a40;
            font-size: 16px;
            padding: 5px;
        }
    )");

    layout->addWidget(textOutput);
    central->setLayout(layout);
    setCentralWidget(central);

    setMaximumSize(MAX_WIDTH, MAX_HEIGHT);
    setMinimumSize(MIN_WIDTH, MIN_HEIGHT);

    // Tray
    QSystemTrayIcon *trayIcon = new QSystemTrayIcon();
    trayIcon->setIcon(QIcon(":images/translate.png"));
    trayIcon->setToolTip("Translate запущен в фоне");
    trayIcon->setVisible(true);

    QMenu *contextTrayMenu = new QMenu(this);
    QAction *quitAction = new QAction("Выход", this);
    contextTrayMenu->addAction(quitAction);
    trayIcon->setContextMenu(contextTrayMenu);

    connect(quitAction, &QAction::triggered, qApp, &QApplication::quit);
}

MainWindow::~MainWindow() {}

void MainWindow::clearText()
{
    textOutput->clear();
    resize(MIN_WIDTH, MIN_HEIGHT);
}

void MainWindow::appendText(const QString &text)
{
    textOutput->append(text);
    adjustWindowSize();
}

void MainWindow::adjustWindowSize()
{
    textOutput->document()->adjustSize();
    QSizeF docSize = textOutput->document()->size();

    int idealWidth = static_cast<int>(docSize.width()) + 40;
    int idealHeight = static_cast<int>(docSize.height()) + 20;

    idealWidth = qBound(MIN_WIDTH, idealWidth, MAX_WIDTH);
    idealHeight = qBound(MIN_HEIGHT, idealHeight, MAX_HEIGHT);

    resize(idealWidth, idealHeight);
}

void MainWindow::showNearSelectedText()
{
    QPoint cursorPos = QCursor::pos();
    QScreen *screen = QGuiApplication::screenAt(cursorPos);
    if (!screen) screen = QGuiApplication::primaryScreen();

    QRect screenGeometry = screen->geometry();

    int x = cursorPos.x();
    int y = cursorPos.y() + 20;

    if (x + width() > screenGeometry.right()) {
        x = screenGeometry.right() - width();
    }
    if (y + height() > screenGeometry.bottom()) {
        y = cursorPos.y() - height() - 10;
    }

    move(x, y);
    show();
    raise();
    activateWindow();
}
