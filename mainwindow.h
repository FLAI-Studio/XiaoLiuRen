#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSlider>
#include <QLabel>
#include <QTextEdit>
#include <QPushButton>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onDivine();
    void onRandom();

private:
    QSlider *monthSlider;
    QLabel *monthLabel;

    QSlider *daySlider;
    QLabel *dayLabel;

    QSlider *hourSlider;
    QLabel *hourLabel;

    QTextEdit *resultText;
};

#endif // MAINWINDOW_H
