#include "mainwindow.h"
#include "core.h"
#include <cstdlib>
#include <ctime>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    srand(static_cast<unsigned>(time(nullptr)));

    setWindowTitle("九宫小六壬 v1.0.0");
    resize(500, 500);

    // 中央部件
    QWidget* central = new QWidget(this);
    setCentralWidget(central);
    QVBoxLayout* layout = new QVBoxLayout(central);

    // 标题
    QLabel* title = new QLabel("🔮 九宫小六壬");
    title->setStyleSheet("font-size: 24px; font-weight: bold; margin: 10px;");
    title->setAlignment(Qt::AlignCenter);
    layout->addWidget(title);

    // 输入行
    QHBoxLayout* inputLayout = new QHBoxLayout();
    monthCombo = new QComboBox();
    dayCombo = new QComboBox();
    hourCombo = new QComboBox();

    for (int i = 1; i <= 12; i++) monthCombo->addItem(QString::number(i));
    for (int i = 1; i <= 30; i++) dayCombo->addItem(QString::number(i));
    for (int i = 1; i <= 24; i++) hourCombo->addItem(QString::number(i));

    inputLayout->addWidget(new QLabel("月:"));
    inputLayout->addWidget(monthCombo);
    inputLayout->addWidget(new QLabel("日:"));
    inputLayout->addWidget(dayCombo);
    inputLayout->addWidget(new QLabel("时:"));
    inputLayout->addWidget(hourCombo);
    layout->addLayout(inputLayout);

    // 按钮行
    QHBoxLayout* btnLayout = new QHBoxLayout();
    QPushButton* divineBtn = new QPushButton("开始占卜");
    QPushButton* randomBtn = new QPushButton("🎲 随机起卦");
    btnLayout->addWidget(divineBtn);
    btnLayout->addWidget(randomBtn);
    layout->addLayout(btnLayout);

    // 结果显示
    resultText = new QTextEdit();
    resultText->setReadOnly(true);
    resultText->setPlaceholderText("结果将显示在这里...");
    layout->addWidget(resultText);

    // 免责
    QLabel* footer = new QLabel("本程序仅为一个数字小游戏，内容仅供娱乐；封建迷信不可取，请相信科学。");
    footer->setStyleSheet("color: gray; font-size: 11px;");
    footer->setWordWrap(true);
    layout->addWidget(footer);

    // 信号连接
    connect(divineBtn, &QPushButton::clicked, this, &MainWindow::onDivine);
    connect(randomBtn, &QPushButton::clicked, this, &MainWindow::onRandom);
}

void MainWindow::onDivine() {
    int m = monthCombo->currentIndex() + 1;
    int d = dayCombo->currentIndex() + 1;
    int h = hourCombo->currentIndex() + 1;

    int s1, s2, s3;
    divinate(m, d, h, s1, s2, s3);

    const Palace& p1 = PALACES[s1 - 1];
    const Palace& p2 = PALACES[s2 - 1];
    const Palace& p3 = PALACES[s3 - 1];

    QString result = QString(
        "========== 占卜结果 ==========\n\n"
        "第一步(月:%1): %2 —— %3\n\n"
        "第二步(日:%4): %5 —— %6\n\n"
        "第三步(时:%7): %8 —— %9\n\n"
        "---------------------------------\n"
        "🔮 综合断语：%10\n"
        "================================="
    ).arg(m).arg(p1.name).arg(p1.meaning)
     .arg(d).arg(p2.name).arg(p2.meaning)
     .arg(h).arg(p3.name).arg(p3.meaning)
     .arg(p3.advice);

    resultText->setText(result);
}

void MainWindow::onRandom() {
    monthCombo->setCurrentIndex(rand() % 12);
    dayCombo->setCurrentIndex(rand() % 30);
    hourCombo->setCurrentIndex(rand() % 24);
    onDivine();
}