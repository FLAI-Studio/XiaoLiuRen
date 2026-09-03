#pragma once

#include <QMainWindow>
#include <QSlider>
#include <QLabel>
#include <QPushButton>
#include <QTextEdit>
#include <vector>
#include <QString>

// ============================================================
//  九宫数据结构
// ============================================================
struct Palace {
    QString name;        // 宫名
    QString nature;      // 吉 / 凶 / 平
    QString meaning;     // 一句话释义
    QString detail;      // 典故 / 详细解说
    QString advice;      // 综合断语（末宫建议）
    QString goodFor;     // 宜
    QString badFor;      // 忌
    QString color;       // 吉凶主题色
};

// ============================================================
//  主窗口
// ============================================================
class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);

private slots:
    void onDivinate();      // 手动占卜
    void onRandom();        // 随机起卦
    void onClear();         // 清空记录

private:
    static constexpr int MODULO = 9;
    static const Palace PALACES[MODULO];

    int calcStep(int base, int add) const;
    void runDivination(int month, int day, int hour);
    QString comprehensive(int s1, int s2, int s3) const;
    QString natureOf(int step) const;
    void replaceCard(QLabel* card, const QString& title, const Palace& p);

    // 输入
    QSlider*   monthSlider = nullptr;
    QSlider*   daySlider   = nullptr;
    QSlider*   hourSlider  = nullptr;
    QLabel*    monthLabel  = nullptr;
    QLabel*    dayLabel    = nullptr;
    QLabel*    hourLabel   = nullptr;

    // 结果
    QLabel*    card[3]     = {nullptr, nullptr, nullptr};
    QLabel*    summaryLabel = nullptr;
    QTextEdit* historyView  = nullptr;

    // 数据
    std::vector<QString> history;
};
