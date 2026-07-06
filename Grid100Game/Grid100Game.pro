QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Static build
contains(QT_CONFIG, static) {
    message("Configuring for STATIC Qt build")
    CONFIG += static

    # Link MinGW runtime statically (if using MinGW)
    QMAKE_CXXFLAGS_RELEASE += -static -static-libgcc -static-libstdc++
    QMAKE_LFLAGS_RELEASE += -static -static-libgcc -static-libstdc++

    # Include static platform plugin
    QTPLUGIN += qwindows
    CONFIG += qt plugin
}
else {
    message("Configuring for DYNAMIC Qt build")
    # Dynamic build: nothing extra needed
}

SOURCES += \
    GameState.cpp \
    Styles.cpp \
    main.cpp \
    mainwindow.cpp \
    settingsdialog.cpp

HEADERS += \
    Cell.h \
    GameState.h \
    Resources.h \
    Styles.h \
    mainwindow.h \
    settingsdialog.h

FORMS += \
    mainwindow.ui \
    settingsdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc

RC_FILE = resources.rc

# Detect if Qt is static
contains(QT_CONFIG, static) {
    QMAKE_LFLAGS += -static -static-libgcc -static-libstdc++
}
