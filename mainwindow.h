#pragma once
#include <QMainWindow>
#include <QComboBox>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget* parent = nullptr);

private slots:
    void onDivine();
    void onRandom();

private:
    QComboBox* monthCombo;
    QComboBox* dayCombo;
    QComboBox* hourCombo;
    QTextEdit* resultText;
};