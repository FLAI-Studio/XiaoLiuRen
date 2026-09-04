#include "mainwindow.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QDate>
#include <QTime>
#include <QRandomGenerator>
#include <QSettings>
#include <QMessageBox>
#include <QApplication>
#include <QDir>

// ==================== 构造函数 ====================
MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    setWindowTitle("九宫小六壬");
    resize(520, 820);

    // ---------- 顶部标题 ----------
    QLabel* title = new QLabel("🔮 九宫小六壬");
    title->setAlignment(Qt::AlignCenter);
    QFont titleFont = title->font();
    titleFont.setPointSize(22);
    titleFont.setBold(true);
    title->setFont(titleFont);

    QLabel* subtitle = new QLabel("掌中掐算 · 诸葛马前课 · 仅供娱乐");
    subtitle->setAlignment(Qt::AlignCenter);
    subtitle->setStyleSheet("color: #888; font-size: 12px;");

    // ---------- 输入区 ----------
    auto makeRow = [this](const QString& text, QSlider*& slider, QLabel*& label, int max) {
        slider = new QSlider(Qt::Horizontal);
        slider->setRange(1, max);
        slider->setValue(1);
        slider->setTickInterval(1);
        slider->setTickPosition(QSlider::TicksBelow);
        label = new QLabel("1");
        label->setFixedWidth(32);
        label->setAlignment(Qt::AlignCenter);
        connect(slider, &QSlider::valueChanged, this, [label](int v) {
            label->setText(QString::number(v));
        });
        QHBoxLayout* row = new QHBoxLayout();
        row->addWidget(new QLabel(text), 0);
        row->addWidget(slider, 1);
        row->addWidget(label, 0);
        return row;
    };

    QVBoxLayout* inputLayout = new QVBoxLayout();
    inputLayout->setSpacing(10);
    inputLayout->addLayout(makeRow("月 (1-12)：", monthSlider, monthLabel, 12));
    inputLayout->addLayout(makeRow("日 (1-30)：", daySlider, dayLabel, 30));
    inputLayout->addLayout(makeRow("时 (1-24)：", hourSlider, hourLabel, 24));

    QGroupBox* inputBox = new QGroupBox("起卦参数");
    inputBox->setLayout(inputLayout);

    // ---------- 按钮区 ----------
    QPushButton* btnDivinate = new QPushButton("✅开始占卜");
    QPushButton* btnRandom   = new QPushButton("🎲随机起卦");
    QPushButton* btnToday    = new QPushButton("📅今日运势");
    QPushButton* btnClear    = new QPushButton("❌清空");
    btnDivinate->setMinimumHeight(44);
    btnRandom->setMinimumHeight(44);
    btnToday->setMinimumHeight(44);
    btnClear->setMinimumHeight(44);
    connect(btnDivinate, &QPushButton::clicked, this, &MainWindow::onDivinate);
    connect(btnRandom,   &QPushButton::clicked, this, &MainWindow::onRandom);
    connect(btnToday,    &QPushButton::clicked, this, &MainWindow::onToday);
    connect(btnClear,    &QPushButton::clicked, this, &MainWindow::onClear);

    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnLayout->addWidget(btnDivinate);
    btnLayout->addWidget(btnRandom);
    btnLayout->addWidget(btnToday);
    btnLayout->addWidget(btnClear);

    // ---------- 九宫格可视化 ----------
    QGroupBox* gridBox = new QGroupBox("九宫格");
    QGridLayout* gridLayout = new QGridLayout(gridBox);
    gridLayout->setSpacing(4);
    for (int i = 0; i < 9; ++i) {
        gridCell[i] = new QLabel(QString::number(i + 1) + "\n" + QString::fromUtf8(PALACES[i].name));
        gridCell[i]->setAlignment(Qt::AlignCenter);
        gridCell[i]->setMinimumSize(60, 60);
        gridCell[i]->setWordWrap(true);
        gridCell[i]->setStyleSheet(
            "border:1px solid #ccc; border-radius:6px; padding:4px; "
            "background:#fafafa; font-size:12px;");
        gridLayout->addWidget(gridCell[i], i / 3, i % 3);
    }

    // ---------- 结果区 ----------
    QVBoxLayout* resultLayout = new QVBoxLayout();
    for (int i = 0; i < 3; ++i) {
        card[i] = new QLabel();
        card[i]->setWordWrap(true);
        card[i]->setMinimumHeight(78);
        resultLayout->addWidget(card[i]);
    }
    summaryLabel = new QLabel("—");
    summaryLabel->setWordWrap(true);
    summaryLabel->setStyleSheet("font-weight: bold; font-size: 14px;");
    resultLayout->addWidget(summaryLabel);

    QGroupBox* resultBox = new QGroupBox("占卜结果");
    resultBox->setLayout(resultLayout);

    // ---------- 历史记录 ----------
    historyList = new QListWidget();
    historyList->setMaximumHeight(130);
    connect(historyList, &QListWidget::itemClicked, this, &MainWindow::onHistoryItemClicked);

    btnDeleteHistory = new QPushButton("🗑删除选中");
    btnDeleteHistory->setEnabled(false);
    connect(btnDeleteHistory, &QPushButton::clicked, this, &MainWindow::onDeleteHistory);

    QVBoxLayout* historyLayout = new QVBoxLayout();
    historyLayout->addWidget(historyList);
    historyLayout->addWidget(btnDeleteHistory);
    QGroupBox* historyBox = new QGroupBox("历史记录");
    historyBox->setLayout(historyLayout);

    // ---------- 底部工具栏 ----------
    themeCheck = new QCheckBox("🌙暗色模式");
    connect(themeCheck, &QCheckBox::toggled, this, &MainWindow::onToggleTheme);

    QHBoxLayout* toolLayout = new QHBoxLayout();
    toolLayout->addWidget(themeCheck);
    toolLayout->addStretch();

    // ---------- 底部信息 ----------
    QLabel* footer = new QLabel(
        "本程序仅为一个数字小游戏，内容仅供娱乐；封建迷信不可取，请相信科学\n\n"
        "版本：v0.1.3\n"
        "开发者：Byjsmc\n"
        "最后更新于：2026/09/05"
        );
    footer->setStyleSheet("color: gray; font-size: 11px;");
    footer->setWordWrap(true);

    // ---------- 主布局 ----------
    QWidget* central = new QWidget();
    QVBoxLayout* mainLayout = new QVBoxLayout(central);
    mainLayout->setSpacing(8);
    mainLayout->addWidget(title);
    mainLayout->addWidget(subtitle);
    mainLayout->addWidget(inputBox);
    mainLayout->addLayout(btnLayout);
    mainLayout->addWidget(gridBox);
    mainLayout->addWidget(resultBox);
    mainLayout->addWidget(historyBox);
    mainLayout->addLayout(toolLayout);
    mainLayout->addWidget(footer);
    setCentralWidget(central);

    // ---------- 加载配置 ----------
    loadConfig();
    loadHistory();
    applyTheme(darkTheme);
}

MainWindow::~MainWindow() {
    saveConfig();
    saveHistory();
}

// ==================== 工具函数 ====================

QString MainWindow::natureColor(const char* nature) const {
    QString n = QString::fromUtf8(nature);
    if (n == "吉") return "#2e7d32";
    if (n == "凶") return "#c62828";
    return "#ef6c00";
}

QString MainWindow::comprehensive(int s1, int s2, int s3) const {
    int good = 0, bad = 0;
    int steps[3] = {s1, s2, s3};
    for (int i = 0; i < 3; ++i) {
        QString n = QString::fromUtf8(PALACES[steps[i] - 1].nature);
        if (n == "吉")      good++;
        else if (n == "凶") bad++;
    }
    if (good == 3) return "三宫皆吉！运势亨通，诸事顺遂，放手作为必有所获。";
    if (bad == 3)  return "三宫皆凶。运势低迷，宜静不宜动，凡事忍让、暂缓决策。";
    if (good == 2) return "两吉一凶。整体向好，但暗藏阻碍，后期需谨慎应对。";
    if (bad == 2)  return "两凶一吉。先难后易，初期坎坷，坚持终有转机。";
    return "吉凶参半。事在人为，保持平常心，顺势而为即可。";
}

void MainWindow::refreshGrid(int s1, int s2, int s3) {
    int hit[3] = {s1, s2, s3};
    for (int i = 0; i < 9; ++i) {
        QString base = "border:1px solid #ccc; border-radius:6px; padding:4px; "
                       "background:#fafafa; font-size:12px;";
        bool isHit = false;
        QString hitColor;
        for (int k = 0; k < 3; ++k) {
            if (i + 1 == hit[k]) {
                isHit = true;
                hitColor = natureColor(PALACES[i].nature);
            }
        }
        if (isHit) {
            gridCell[i]->setStyleSheet(QString(
                                           "border:2px solid %1; border-radius:6px; padding:4px; "
                                           "background:%1; color:white; font-size:12px; font-weight:bold;")
                                           .arg(hitColor));
        } else {
            gridCell[i]->setStyleSheet(base);
        }
    }
}

void MainWindow::replaceCard(QLabel* card, const QString& title, const Palace& p) {
    QString color = natureColor(p.nature);
    card->setText(QString(
                      "<div style='border-left:5px solid %1; padding:6px 10px; background:#fafafa; border-radius:4px;'>"
                      "<b>%2</b> <span style='color:%1;'>【%3 · %4】</span><br>"
                      "<span style='font-size:13px;'>%5</span><br>"
                      "<span style='color:#555; font-size:11px;'>💡 %6</span>"
                      "</div>"
                      ).arg(color, title,
                           QString::fromUtf8(p.nature),
                           QString::fromUtf8(p.name),
                           QString::fromUtf8(p.meaning),
                           QString::fromUtf8(p.advice)));
}

// ==================== 占卜逻辑 ====================

void MainWindow::onDivinate() {
    runDivination(monthSlider->value(), daySlider->value(), hourSlider->value());
}

void MainWindow::onRandom() {
    int month = QRandomGenerator::global()->bounded(1, 13);
    int day   = QRandomGenerator::global()->bounded(1, 31);
    int hour  = QRandomGenerator::global()->bounded(1, 25);
    monthSlider->setValue(month);
    daySlider->setValue(day);
    hourSlider->setValue(hour);
    runDivination(month, day, hour);
}

void MainWindow::onToday() {
    QDate today = QDate::currentDate();
    QTime now = QTime::currentTime();
    int month = today.month();
    int day   = today.day();
    int hour  = now.hour() + 1;
    if (hour > 24) hour = 24;

    monthSlider->setValue(month);
    daySlider->setValue(day);
    hourSlider->setValue(hour);
    runDivination(month, day, hour);
}

void MainWindow::runDivination(int month, int day, int hour) {
    int s1, s2, s3;
    divinate(month, day, hour, s1, s2, s3);

    QString titles[3] = {
        QString("初宫（月 %1 · 起因）").arg(month),
        QString("中宫（日 %1 · 过程）").arg(day),
        QString("末宫（时 %1 · 结局）").arg(hour)
    };
    replaceCard(card[0], titles[0], PALACES[s1 - 1]);
    replaceCard(card[1], titles[1], PALACES[s2 - 1]);
    replaceCard(card[2], titles[2], PALACES[s3 - 1]);

    refreshGrid(s1, s2, s3);

    summaryLabel->setText(QString("🔮 %1\n📌 末宫建议：%2")
                              .arg(comprehensive(s1, s2, s3),
                                   QString::fromUtf8(PALACES[s3 - 1].advice)));

    QString record = QString("[%1] %2/%3 %4时 → %5 · %6 · %7")
                         .arg(QTime::currentTime().toString("HH:mm:ss"))
                         .arg(month).arg(day).arg(hour)
                         .arg(QString::fromUtf8(PALACES[s1 - 1].name),
                              QString::fromUtf8(PALACES[s2 - 1].name),
                              QString::fromUtf8(PALACES[s3 - 1].name));
    history.append(record);
    historyList->addItem(record);

    saveHistory();
}

// ==================== 清空 ====================

void MainWindow::onClear() {
    QMessageBox::StandardButton ret =
        QMessageBox::question(this, "确认清空",
                              "确定要清空所有历史记录吗？此操作不可撤销。",
                              QMessageBox::Yes | QMessageBox::No);
    if (ret != QMessageBox::Yes) return;

    for (int i = 0; i < 3; ++i) card[i]->clear();
    summaryLabel->clear();
    historyList->clear();
    history.clear();
    refreshGrid(0, 0, 0);
    saveHistory();
}

// ==================== 历史记录管理 ====================

void MainWindow::onHistoryItemClicked(QListWidgetItem* item) {
    Q_UNUSED(item);
    btnDeleteHistory->setEnabled(true);
}

void MainWindow::onDeleteHistory() {
    int row = historyList->currentRow();
    if (row < 0 || row >= historyList->count()) return;

    QMessageBox::StandardButton ret =
        QMessageBox::question(this, "删除历史",
                              "确定删除这条记录吗？",
                              QMessageBox::Yes | QMessageBox::No);
    if (ret != QMessageBox::Yes) return;

    history.removeAt(row);
    delete historyList->takeItem(row);
    if (historyList->count() == 0) btnDeleteHistory->setEnabled(false);
    saveHistory();
}

// ==================== 暗色模式 ====================

void MainWindow::onToggleTheme() {
    darkTheme = themeCheck->isChecked();
    applyTheme(darkTheme);
    saveConfig();
}

void MainWindow::applyTheme(bool dark) {
    if (dark) {
        qApp->setStyleSheet(
            "QMainWindow, QWidget { background-color: #2b2b2b; color: #eee; }"
            "QGroupBox { border: 1px solid #555; border-radius: 8px; "
            "            font-weight: bold; font-size: 14px; margin-top: 8px; color: #ddd; }"
            "QGroupBox::title { subcontrol-origin: margin; left: 12px; padding: 0 4px; }"
            "QSlider::groove:horizontal { height: 10px; background: #555; border-radius: 5px; }"
            "QSlider::handle:horizontal { width: 40px; height: 40px; "
            "            background: #5b8def; border-radius: 20px; margin: -15px 0; }"
            "QPushButton { background: #3a6bc7; color: white; border-radius: 9px; font-size: 15px; }"
            "QPushButton:pressed { background: #2a4fa0; }"
            "QPushButton:disabled { background: #444; color: #888; }"
            "QListWidget { background: #353535; color: #eee; border: 1px solid #555; border-radius: 4px; }"
            "QLabel { color: #eee; }"
            );
    } else {
        qApp->setStyleSheet(
            "QSlider::groove:horizontal { height: 10px; background: #ddd; border-radius: 5px; }"
            "QSlider::handle:horizontal { width: 40px; height: 40px; "
            "            background: #5b8def; border-radius: 20px; margin: -15px 0; }"
            "QPushButton { background: #5b8def; color: white; border-radius: 9px; font-size: 15px; }"
            "QPushButton:pressed { background: #3a6bc7; }"
            "QGroupBox { font-weight: bold; font-size: 14px; border: 1px solid #ccc; "
            "            border-radius: 8px; margin-top: 8px; }"
            "QGroupBox::title { subcontrol-origin: margin; left: 12px; padding: 0 4px; }"
            );
    }
}

// ==================== 配置持久化 ====================

void MainWindow::saveConfig() {
    QSettings settings("Byjsmc", "XiaoLiuRen");
    settings.setValue("darkTheme", darkTheme);
    settings.setValue("month", monthSlider->value());
    settings.setValue("day", daySlider->value());
    settings.setValue("hour", hourSlider->value());
}

void MainWindow::loadConfig() {
    QSettings settings("Byjsmc", "XiaoLiuRen");
    darkTheme = settings.value("darkTheme", false).toBool();
    themeCheck->setChecked(darkTheme);

    int m = settings.value("month", 1).toInt();
    int d = settings.value("day", 1).toInt();
    int h = settings.value("hour", 1).toInt();
    monthSlider->setValue(m);
    daySlider->setValue(d);
    hourSlider->setValue(h);
}

void MainWindow::saveHistory() {
    QSettings settings("Byjsmc", "XiaoLiuRen");
    QStringList list;
    for (const auto& s : history) list.append(s);
    settings.setValue("history", list);
}

void MainWindow::loadHistory() {
    QSettings settings("Byjsmc", "XiaoLiuRen");
    QStringList list = settings.value("history").toStringList();
    history.clear();
    for (const auto& s : list) {
        history.append(s);
        historyList->addItem(s);
    }
}
