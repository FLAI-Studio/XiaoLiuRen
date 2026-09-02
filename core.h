#ifndef CORE_H
#define CORE_H

// 九宫数据
struct Palace {
    const char* name;
    const char* nature;
    const char* meaning;
    const char* advice;
};

static const Palace PALACES[9] = {
    {"大安", "吉", "青龙临位，安稳无忧", "事情平稳，安心等待即可。"},
    {"留连", "凶", "玄武纠缠，拖延阻碍", "事情有拖延，需耐心应对阻力。"},
    {"速喜", "吉", "朱雀报喜，快速有成", "好运来得快，抓紧行动别犹豫！"},
    {"赤口", "凶", "白虎口舌，谨防是非", "易有口舌之争，谨言慎行保平安。"},
    {"小吉", "吉", "六合和合，平平顺遂", "小有所成，保持平常心即可。"},
    {"空亡", "凶", "勾陈落空，时机未到", "此事恐落空，不如另做打算。"},
    {"大吉", "吉", "大吉大利，万事亨通", "鸿运当头，放手去做必有所获！"},
    {"小安", "平", "小有所成，安稳无虞", "安稳度日，小惊喜正在路上。"},
    {"速断", "平", "当机立断，速战速决", "犹豫就会败北，立刻做决定！"}
};

// 核心算法
inline void divinate(int month, int day, int hour, int& s1, int& s2, int& s3) {
    auto calcStep = [](int base, int add) -> int {
        int r = (base + add) % 9;
        return r == 0 ? 9 : r;
    };
    s1 = calcStep(month % 9, 0);
    s2 = calcStep(s1, day);
    s3 = calcStep(s2, hour);
}

#endif // CORE_H