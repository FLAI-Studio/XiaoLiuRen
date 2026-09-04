QT += core gui widgets
TARGET = XiaoLiuRen
TEMPLATE = app

SOURCES += main.cpp mainwindow.cpp
HEADERS += mainwindow.h core.h \
    core.h

# 安卓配置
android {
    ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
    ANDROID_ABIS = arm64-v8a
}
