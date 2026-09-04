#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSlider>
#include <QLabel>
#include <QPushButton>
#include <QListWidget>
#include <QCheckBox>
#include <QVector>
#include <QString>

#include "core.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void onDivinate();
    void onRandom();
    void onToday();
    void onClear();
    void onToggleTheme();
    void onHistoryItemClicked(QListWidgetItem* item);
    void onDeleteHistory();

private:
    QString natureColor(const char* nature) const;
    QString comprehensive(int s1, int s2, int s3) const;
    void refreshGrid(int s1, int s2, int s3);
    void replaceCard(QLabel* card, const QString& title, const Palace& p);
    void saveConfig();
    void loadConfig();
    void saveHistory();
    void loadHistory();
    void applyTheme(bool dark);
    void runDivination(int month, int day, int hour);

    // UI
    QSlider*   monthSlider   = nullptr;
    QSlider*   daySlider     = nullptr;
    QSlider*   hourSlider    = nullptr;
    QLabel*    monthLabel    = nullptr;
    QLabel*    dayLabel      = nullptr;
    QLabel*    hourLabel     = nullptr;
    QLabel*    card[3]       = {nullptr, nullptr, nullptr};
    QLabel*    summaryLabel  = nullptr;
    QLabel*    gridCell[9]   = {nullptr};
    QListWidget* historyList  = nullptr;
    QPushButton*  btnDeleteHistory = nullptr;
    QCheckBox* themeCheck    = nullptr;

    // 数据
    QVector<QString> history;
    bool darkTheme    = false;
};

#endif // MAINWINDOW_H
