#include "mainwindow.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QDateTime>
#include <QRandomGenerator>
#include <QSettings>

// ============================================================
//  九宫数据定义
//  顺序：大安(0) 留连(1) 速喜(2) 赤口(3) 小吉(4) 空亡(5) 大吉(6) 小安(7) 速断(8)
//  每个宫位包含：名称 / 吉凶 / 释义 / 详解 / 建议 / 宜 / 忌 / 主题色
// ============================================================
const Palace MainWindow::PALACES[MODULO] = {
    {"大安", "吉",
     "安稳顺利，不动为吉。",
     "六壬之首，主静守。凡事平稳、无风无浪，宜安居乐业、守旧待时，忌妄动冒进。",
     "运势平稳，宜守不宜攻，静待时机即可。",
     "守成、签约、求稳、居家", "冒险、远行、急进",
     "#2e7d32"},

    {"留连", "凶",
     "拖延纠缠，暂缓行事。",
     "主纠缠、反复、拖延。事情易受阻停滞，迟迟难决，宜耐心等待、理清头绪再行动。",
     "事情有阻碍，建议先放一放，理清再说。",
     "复盘、沟通、暂缓决策", "仓促定论、强推硬上",
     "#c62828"},

    {"速喜", "吉",
     "快速喜庆，好事将近。",
     "主迅速、喜讯。好事来得快，多有意外之喜、消息将至，宜抓紧行动莫犹豫。",
     "好运来得快，抓紧行动别犹豫！",
     "求财、赴约、表白、出行", "拖延、坐失良机",
     "#2e7d32"},

    {"赤口", "凶",
     "口舌争斗，谨防是非。",
     "主口舌、争讼、是非。易有争吵官非、言语误会，宜谨言慎行、以和为贵。",
     "易有口舌之争，谨言慎行保平安。",
     "沉默、和解、签字（需谨慎）", "争吵、许诺、争执",
     "#c62828"},

    {"小吉", "吉",
     "和合小成，平平顺遂。",
     "主和合、小成。事情多有小成，人际和睦，宜与人合作、顺势而为。",
     "小有所成，保持平常心即可。",
     "合作、交友、小事、牵线", "独断、贪大求全",
     "#2e7d32"},

    {"空亡", "凶",
     "落空虚无，时机未到。",
     "主落空、虚幻、无果。劳而无功、期望落空，宜另做打算、积蓄待发。",
     "此事恐落空，不如另做打算。",
     "止损、等待、调整方向", "死磕、大额投资、强求",
     "#c62828"},

    {"大吉", "吉",
     "大吉大利，万事亨通。",
     "六壬最吉之宫。运势亨通、诸事顺遂，所求皆成，宜放手大胆作为。",
     "鸿运当头，放手去做必有所获！",
     "开创、求财、婚嫁、大事", "犹豫、退缩",
     "#2e7d32"},

    {"小安", "平",
     "小有所成，安稳无虞。",
     "主安稳、小成。无大喜亦无大忧，平淡是福，宜踏实经营、静养身心。",
     "安稳度日，小惊喜正在路上。",
     "日常、养生、积攒、学习", "投机、冒进",
     "#ef6c00"},

    {"速断", "平",
     "当机立断，速战速决。",
     "主决断、明快。犹豫则生变，宜当机立断、速战速决，切勿拖泥带水。",
     "犹豫就会败北，立刻做决定！",
     "决断、止损、快刀斩乱麻", "拖延、反复纠结",
     "#ef6c00"},
    };

// ============================================================
//  构造函数：UI 布局与初始化
// ============================================================
MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    setWindowTitle("九宫小六壬");
    resize(460, 760);

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

    // ---------- 输入区（滑条）----------
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
    inputLayout->addLayout(makeRow("日 (1-30)：", daySlider,   dayLabel,   30));
    inputLayout->addLayout(makeRow("时 (1-24)：", hourSlider,  hourLabel,  24));

    QGroupBox* inputBox = new QGroupBox("起卦参数");
    inputBox->setLayout(inputLayout);

    // ---------- 按钮区 ----------
    QPushButton* btnDivinate = new QPushButton("✅开始占卜");
    QPushButton* btnRandom   = new QPushButton("🎲随机起卦");
    QPushButton* btnClear    = new QPushButton("❌清空");
    btnDivinate->setMinimumHeight(44);
    btnRandom->setMinimumHeight(44);
    btnClear->setMinimumHeight(44);
    connect(btnDivinate, &QPushButton::clicked, this, &MainWindow::onDivinate);
    connect(btnRandom,   &QPushButton::clicked, this, &MainWindow::onRandom);
    connect(btnClear,    &QPushButton::clicked, this, &MainWindow::onClear);

    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnLayout->addWidget(btnDivinate);
    btnLayout->addWidget(btnRandom);
    btnLayout->addWidget(btnClear);

    // ---------- 结果区（三宫卡片 + 综合断语）----------
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
    historyView = new QTextEdit();
    historyView->setReadOnly(true);
    historyView->setMaximumHeight(130);
    QGroupBox* historyBox = new QGroupBox("历史记录");
    QVBoxLayout* historyLayout = new QVBoxLayout();
    historyLayout->addWidget(historyView);
    historyBox->setLayout(historyLayout);

    // ---------- 底部信息 ----------
    QLabel* footer = new QLabel(
        "本程序仅为一个数字小游戏，内容仅供娱乐；封建迷信不可取，请相信科学\n\n"
        "版本：v0.1.1\n"
        "开发者：Byjsmc\n"
        "最后更新于：2026/09/03"
        );
    footer->setStyleSheet("color: gray; font-size: 11px;");
    footer->setWordWrap(true);

    // ---------- 主布局组装 ----------
    QWidget* central = new QWidget();
    QVBoxLayout* mainLayout = new QVBoxLayout(central);
    mainLayout->setSpacing(10);
    mainLayout->addWidget(title);
    mainLayout->addWidget(subtitle);
    mainLayout->addWidget(inputBox);
    mainLayout->addLayout(btnLayout);
    mainLayout->addWidget(resultBox);
    mainLayout->addWidget(historyBox);
    mainLayout->addWidget(footer);
    setCentralWidget(central);

    // ---------- 全局样式表（安卓触屏优化）----------
    setStyleSheet(
        "QSlider::groove:horizontal { height: 10px; background: #ddd; border-radius: 5px; }"
        "QSlider::handle:horizontal  { width: 40px; height: 40px; background: #5b8def; border-radius: 20px; margin: -15px 0; }"
        "QPushButton { background: #5b8def; color: white; border-radius: 9px; font-size: 15px; }"
        "QPushButton:pressed { background: #3a6bc7; }"
        "QGroupBox { font-weight: bold; font-size: 14px; border: 1px solid #ccc; border-radius: 8px; margin-top: 8px; }"
        "QGroupBox::title { subcontrol-origin: margin; left: 12px; padding: 0 4px; }"
        );
loadHistory();
}

// ============================================================
//  算法工具
// ============================================================

/**
 * @brief 九宫取模计算
 * @param base 上一宫结果
 * @param add  本次输入值
 * @return 1~9 的宫位编号
 */
int MainWindow::calcStep(int base, int add) const {
    int res = (base + add) % MODULO;
    return res == 0 ? MODULO : res;
}

/**
 * @brief 根据宫位编号获取吉凶性质
 * @param step 1~9
 * @return "吉" / "凶" / "平"
 */
QString MainWindow::natureOf(int step) const {
    return PALACES[step - 1].nature;
}

/**
 * @brief 综合断语：统计三宫吉凶数量，给出整体判断
 */
QString MainWindow::comprehensive(int s1, int s2, int s3) const {
    int good = 0, bad = 0;
    int steps[3] = {s1, s2, s3};
    for (int i = 0; i < 3; ++i) {
        QString n = natureOf(steps[i]);
        if (n == "吉")      good++;
        else if (n == "凶") bad++;
    }
    if (good == 3) return "三宫皆吉！运势亨通，诸事顺遂，放手作为必有所获。";
    if (bad == 3)  return "三宫皆凶。运势低迷，宜静不宜动，凡事忍让、暂缓决策。";
    if (good == 2) return "两吉一凶。整体向好，但暗藏阻碍，后期需谨慎应对。";
    if (bad == 2)  return "两凶一吉。先难后易，初期坎坷，坚持终有转机。";
    return "吉凶参半。事在人为，保持平常心，顺势而为即可。";
}

/**
 * @brief 更新卡片内容（复用 QLabel，避免重新布局）
 */
void MainWindow::replaceCard(QLabel* card, const QString& title, const Palace& p) {
    card->setText(QString(
                      "<div style='border-left:5px solid %1; padding:6px 10px; background:#fafafa; border-radius:4px;'>"
                      "<b>%2</b> <span style='color:%1;'>【%3】</span><br>"
                      "<span style='font-size:13px;'>%4</span><br>"
                      "<span style='color:#555; font-size:11px;'>宜：%5 ｜ 忌：%6</span>"
                      "</div>"
                      ).arg(p.color, title, p.nature + " · " + p.name, p.detail, p.goodFor, p.badFor));
}

// ============================================================
//  占卜逻辑
// ============================================================

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

void MainWindow::runDivination(int month, int day, int hour) {
    int step1 = calcStep(month % MODULO, 0);
    int step2 = calcStep(step1, day);
    int step3 = calcStep(step2, hour);

    const Palace& p1 = PALACES[step1 - 1];
    const Palace& p2 = PALACES[step2 - 1];
    const Palace& p3 = PALACES[step3 - 1];

    // 三宫卡片（初·起因 / 中·过程 / 末·结局）
    QString titles[3] = {
        QString("初宫（月 %1 · 起因）").arg(month),
        QString("中宫（日 %1 · 过程）").arg(day),
        QString("末宫（时 %1 · 结局）").arg(hour)
    };
    replaceCard(card[0], titles[0], p1);
    replaceCard(card[1], titles[1], p2);
    replaceCard(card[2], titles[2], p3);

    // 综合断语 = 三宫吉凶统计 + 末宫建议
    summaryLabel->setText(QString("🔮 %1\n📌 末宫建议：%2")
                              .arg(comprehensive(step1, step2, step3), p3.advice));

    // 历史记录
    QString record = QString("[%1] %2/%3 %4时 → %5 · %6 · %7")
                         .arg(QDateTime::currentDateTime().toString("HH:mm:ss"))
                         .arg(month).arg(day).arg(hour)
                         .arg(p1.name, p2.name, p3.name);
    history.push_back(record);
    historyView->append(record);

    saveHistory();
}

// ============================================================
//  清空
// ============================================================

void MainWindow::onClear() {
    for (int i = 0; i < 3; ++i) {
        if (card[i]) card[i]->clear();
    }
    if (summaryLabel) summaryLabel->clear();
    if (historyView)  historyView->clear();
    history.clear();

    saveHistory();
}

// ============================================================
//  本地存储历史记录
// ============================================================
void MainWindow::saveHistory() {
    QSettings settings("Byjsmc", "XiaoLiuRen");
    // 把 vector<QString> 转成 QStringList 存下来
    QStringList list;
    for (const auto& s : history) {
        list.append(s);
    }
    settings.setValue("history", list);
    // 同时记住当前滑条位置（下次打开直接恢复）
    settings.setValue("month", monthSlider->value());
    settings.setValue("day", daySlider->value());
    settings.setValue("hour", hourSlider->value());
}

void MainWindow::loadHistory() {
    QSettings settings("Byjsmc", "XiaoLiuRen");
    QStringList list = settings.value("history").toStringList();
    history.clear();
    for (const auto& s : list) {
        history.push_back(s);
        historyView->append(s);
    }
    // 恢复上次滑条位置
    int m = settings.value("month", 1).toInt();
    int d = settings.value("day", 1).toInt();
    int h = settings.value("hour", 1).toInt();
    monthSlider->setValue(m);
    daySlider->setValue(d);
    hourSlider->setValue(h);
}
