#include "mainwindow.h"
#include "core.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <cstdlib>
#include <ctime>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    srand(static_cast<unsigned>(time(nullptr)));

    setWindowTitle("九宫小六壬");
    resize(500, 520);

    QWidget* central = new QWidget(this);
    setCentralWidget(central);
    QVBoxLayout* layout = new QVBoxLayout(central);
    layout->setSpacing(12);
    layout->setContentsMargins(16, 16, 16, 16);

    // ===== 标题 =====
    QLabel* title = new QLabel("🔮 九宫小六壬");
    title->setStyleSheet("font-size: 24px; font-weight: bold; margin: 10px;");
    title->setAlignment(Qt::AlignCenter);
    layout->addWidget(title);

    // ===== 输入区域 =====
    QGroupBox* inputGroup = new QGroupBox("起卦参数");
    QVBoxLayout* inputLayout = new QVBoxLayout(inputGroup);
    inputLayout->setSpacing(10);

    // 月（1-12）
    monthSlider = new QSlider(Qt::Horizontal);
    monthSlider->setRange(1, 12);
    monthSlider->setValue(1);
    monthSlider->setTickInterval(1);
    monthSlider->setTickPosition(QSlider::TicksBelow);
    monthLabel = new QLabel("1");
    monthLabel->setFixedWidth(30);
    monthLabel->setAlignment(Qt::AlignCenter);

    QHBoxLayout* monthLayout = new QHBoxLayout;
    monthLayout->addWidget(new QLabel("月："));
    monthLayout->addWidget(monthSlider);
    monthLayout->addWidget(monthLabel);

    // 日（1-30）
    daySlider = new QSlider(Qt::Horizontal);
    daySlider->setRange(1, 30);
    daySlider->setValue(1);
    daySlider->setTickInterval(1);
    daySlider->setTickPosition(QSlider::TicksBelow);
    dayLabel = new QLabel("1");
    dayLabel->setFixedWidth(30);
    dayLabel->setAlignment(Qt::AlignCenter);

    QHBoxLayout* dayLayout = new QHBoxLayout;
    dayLayout->addWidget(new QLabel("日："));
    dayLayout->addWidget(daySlider);
    dayLayout->addWidget(dayLabel);

    // 时（1-24）
    hourSlider = new QSlider(Qt::Horizontal);
    hourSlider->setRange(1, 24);
    hourSlider->setValue(1);
    hourSlider->setTickInterval(1);
    hourSlider->setTickPosition(QSlider::TicksBelow);
    hourLabel = new QLabel("1");
    hourLabel->setFixedWidth(30);
    hourLabel->setAlignment(Qt::AlignCenter);

    QHBoxLayout* hourLayout = new QHBoxLayout;
    hourLayout->addWidget(new QLabel("时："));
    hourLayout->addWidget(hourSlider);
    hourLayout->addWidget(hourLabel);

    inputLayout->addLayout(monthLayout);
    inputLayout->addLayout(dayLayout);
    inputLayout->addLayout(hourLayout);
    layout->addWidget(inputGroup);

    // 滑条拖动时更新数字
    connect(monthSlider, &QSlider::valueChanged, this, [this](int v){
        monthLabel->setText(QString::number(v));
    });
    connect(daySlider, &QSlider::valueChanged, this, [this](int v){
        dayLabel->setText(QString::number(v));
    });
    connect(hourSlider, &QSlider::valueChanged, this, [this](int v){
        hourLabel->setText(QString::number(v));
    });

    // ===== 按钮行 =====
    QHBoxLayout* btnLayout = new QHBoxLayout();
    QPushButton* divineBtn = new QPushButton("开始占卜");
    QPushButton* randomBtn = new QPushButton("🎲 随机起卦");
    divineBtn->setMinimumHeight(42);
    randomBtn->setMinimumHeight(42);
    btnLayout->addWidget(divineBtn);
    btnLayout->addWidget(randomBtn);
    layout->addLayout(btnLayout);

    // ===== 结果显示 =====
    resultText = new QTextEdit();
    resultText->setReadOnly(true);
    resultText->setPlaceholderText("结果将显示在这里...");
    layout->addWidget(resultText);

    // ===== 免责 =====
    QLabel* footer = new QLabel("本程序仅为一个数字小游戏，内容仅供娱乐；封建迷信不可取，请相信科学\nv0.0.1 * Byjsmc * 2026/09/03");
    footer->setStyleSheet("color: gray; font-size: 11px;");
    footer->setWordWrap(true);
    layout->addWidget(footer);

    // 信号连接
    connect(divineBtn, &QPushButton::clicked, this, &MainWindow::onDivine);
    connect(randomBtn, &QPushButton::clicked, this, &MainWindow::onRandom);

    // 安卓滑条样式（加粗好拖）
    setStyleSheet(
        "QSlider::groove:horizontal { height: 10px; background: #ddd; border-radius: 5px; }"
        "QSlider::handle:horizontal { width: 28px; height: 28px; background: #5b8def; border-radius: 14px; margin: -9px 0; }"
        "QPushButton { background: #5b8def; color: white; border-radius: 8px; font-size: 16px; }"
        "QPushButton:pressed { background: #3a6bc7; }"
        );
}

MainWindow::~MainWindow() {}

void MainWindow::onDivine() {
    int m = monthSlider->value();
    int d = daySlider->value();
    int h = hourSlider->value();

    int s1, s2, s3;
    divinate(m, d, h, s1, s2, s3);

    const Palace& p1 = PALACES[s1 - 1];
    const Palace& p2 = PALACES[s2 - 1];
    const Palace& p3 = PALACES[s3 - 1];

    // ===== 综合断语 =====
    int indices[3] = {s1 - 1, s2 - 1, s3 - 1};
    int good = 0, bad = 0;
    for (int i = 0; i < 3; i++) {
        int idx = indices[i];
        if (idx == 0 || idx == 2 || idx == 4) good++;
        else if (idx == 3 || idx == 5) bad++;
    }

    QString comprehensive;
    if (good == 3) {
        comprehensive = "🟢 三宫皆吉！大吉大利，诸事顺遂，所求皆如愿。";
    } else if (bad == 3) {
        comprehensive = "🔴 三宫皆凶。运势低迷，宜静不宜动，凡事忍让为上，暂缓决策。";
    } else if (good == 2) {
        comprehensive = "🟡 两吉一凶。整体向好，但暗藏阻碍，后期需谨慎应对。";
    } else if (bad == 2) {
        comprehensive = "🟡 两凶一吉。先难后易，初期坎坷，坚持终有转机。";
    } else {
        comprehensive = "🔵 吉凶参半。事在人为，保持平常心，顺势而为即可。";
    }

    // ===== 拼接结果 =====
    QString result = QString(
                         "========== 占卜结果 ==========\n\n"
                         "【初宫】月%1 → %2\n  %3\n\n"
                         "【中宫】日%4 → %5\n  %6\n\n"
                         "【末宫】时%7 → %8\n  %9\n\n"
                         "---------------------------------\n"
                         "🔮 综合断语：%10\n\n"
                         "📌 末宫建议：%11\n"
                         "================================="
                         ).arg(m).arg(p1.name).arg(p1.meaning)
                         .arg(d).arg(p2.name).arg(p2.meaning)
                         .arg(h).arg(p3.name).arg(p3.meaning)
                         .arg(comprehensive)
                         .arg(p3.advice);

    resultText->setText(result);
}

void MainWindow::onRandom() {
    monthSlider->setValue((rand() % 12) + 1);
    daySlider->setValue((rand() % 30) + 1);
    hourSlider->setValue((rand() % 24) + 1);
    onDivine();
}
