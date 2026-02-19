#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#pragma once

#include <QMainWindow>
#include <QTextEdit>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void appendText(const QString &text);
    void showNearSelectedText();
    void adjustWindowSize();
    void clearText();


private:
    QTextEdit *textOutput;
    const int MAX_WIDTH = 500;
    const int MAX_HEIGHT = 400;
    const int MIN_HEIGHT = 50;
    const int MIN_WIDTH = 100;
};

#endif // MAINWINDOW_H
